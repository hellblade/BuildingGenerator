#define _USE_MATH_DEFINES

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <MathHelper.h>

#include "BuildingGenerator.h"

#ifdef WINDOWS
extern "C"
{
#endif
    #include "gpc.h"
#ifdef WINDOWS
}
#endif

namespace BuildingGenerator
{
    namespace Impl
    {
        void PrintPolygon(gpc_polygon polygon)
        {
            for (int j = 0; j < polygon.num_contours; j++)
            {
                for (int i = 0; i < polygon.contour[j].num_vertices; i++)
                {
                    gpc_vertex vert = polygon.contour[j].vertex[i];
                    printf("%f, %f\n", vert.x, vert.y);
                }
            }
        }

        gpc_polygon BoundPolygon(gpc_polygon &polygon, float x, float y, float width, float height)
        {
            int num = 0;

#if DEBUG
            if (polygon.num_contours > 1)
            {
                printf("ERROR: EXPECTED ONE CONTOUR FOR BOUNDING\n");                
            }
#endif

            for (int j = 0; j < polygon.contour->num_vertices; j++)
            {
                gpc_vertex vert = polygon.contour->vertex[j];
                if (InsideRectangle(vert.x, vert.y, x, y, width, height))
                {
                    num++;
                }
            }

            gpc_polygon result = gpc_create_polygon(num);

            num = 0;
            for (int i = 0; i < polygon.num_contours; i++)
            {
                for (int j = 0; j < polygon.contour[i].num_vertices; j++)
                {
                    gpc_vertex vert = polygon.contour[i].vertex[j];
                    if (InsideRectangle(vert.x, vert.y, x, y, width, height))
                    {
                        result.contour->vertex[num].x = vert.x;
                        result.contour->vertex[num].y = vert.y;
                        num++;
                    }
                }
            }

            return result;
        }

        inline gpc_polygon MergePolygons(gpc_polygon *p1, gpc_polygon *p2, bool freeOne, bool freeTwo)
        {
            gpc_polygon result;
            gpc_polygon_clip(GPC_UNION, p1, p2, &result);

            if (freeOne)
            {
                gpc_free_polygon(p1);
            }

            if (freeTwo)
            {
                gpc_free_polygon(p2);
            }

            return result;
        }

        inline gpc_polygon SubtractPolygons(gpc_polygon *p1, gpc_polygon *p2, bool freeOne, bool freeTwo)
        {
            gpc_polygon result;
            gpc_polygon_clip(GPC_DIFF, p1, p2, &result);

            if (freeOne)
            {
                gpc_free_polygon(p1);
            }

            if (freeTwo)
            {
                gpc_free_polygon(p2);
            }

            return result;
        }

        gpc_polygon GenerateNGon(float centerX, float centerZ, float radius, int numVerts, float offsetX, float offsetZ)
        {
            float angle = 2 * M_PI / numVerts;

            gpc_polygon polygon = gpc_create_polygon(numVerts);

            float theta = Random::Rndf() * angle;
            for (int i = 0; i < numVerts; i++)
            {
                // Idea is to rotate the vector (radius, 0) around by theta
                // The rotation matrix is
                //  |cos th  -sin th|
                //  |sin th   cos th|
                // And it can be simplifed since the second component is zero
                float cosTh = cosf(theta);
                float sinTh = sinf(theta);

                polygon.contour->vertex[numVerts - i - 1].x = radius * cosTh + centerX + offsetX;
                polygon.contour->vertex[numVerts - i - 1].y = radius * sinTh + centerZ + offsetZ;
                theta += angle;
            }

            return polygon;
        }

        void TranslatePolygon(gpc_polygon &polygon, float offsetX, float offsetY)
        {
            for (int i = 0; i < polygon.num_contours; i++)
            {
                for (int j = 0; j < polygon.contour[i].num_vertices; j++)
                {
                    polygon.contour[i].vertex[j].x += offsetX;
                    polygon.contour[i].vertex[j].y += offsetY;
                }
            }
        }

        void GetPolygonCenter(BuildingInfo &info, gpc_polygon polygon, float &midX, float &midZ)
        {
            float maxX = 0, minX = info.width;
            float maxZ = 0, minZ = info.depth;

            for (int j = 0; j < polygon.num_contours; j++)
            {
                for (int i = 0; i < polygon.contour[j].num_vertices; i++)
                {
                    gpc_vertex vert = polygon.contour[j].vertex[i];
                    maxX = Max(maxX, (float)vert.x);
                    minX = Min(minX, (float)vert.x);
                    maxZ = Max(maxZ, (float)vert.y);
                    minZ = Min(minZ, (float)vert.y);
                }

            }

            midX = (maxX + minX) / 2;
            midZ = (maxZ + minZ) / 2;
        }

        bool GetRandomVertexOffset(const BuildingInfo &info, gpc_polygon polygon, float radius, float &outX, float &outZ)
        {
            std::vector<gpc_vertex> unseen;            

            for (int j = 0; j < polygon.num_contours; j++)
            {
                for (int i = 0; i < polygon.contour[j].num_vertices; i++)
                {
                    gpc_vertex vert = polygon.contour[j].vertex[i];
                    unseen.push_back(vert);
                }
            }

            while (unseen.size() > 0)
            {
                int index = Random::Rnd(unseen.size() - 1);

                gpc_vertex *vertex = unseen.data() + index;
                if (vertex->x + radius >=  0 && vertex->x + radius <= info.width && vertex->x - radius >= 0 && vertex->x - radius <= info.width &&
                    vertex->y + radius >= 0 && vertex->y + radius <= info.depth && vertex->y - radius >= 0 && vertex->y - radius <= info.depth)
                {
                    outX = vertex->x - info.width / 2.0f;
                    outZ = vertex->y - info.depth / 2.0f;
                    return true;
                }
                unseen.erase(unseen.begin() + index);
            }
            return false;
        }

        BuildingVertex GetBuildingVertex(gpc_vertex vert, float height, float c, float texX, float texY, float texZ, int scaleX, int scaleZ)
        {
            BuildingVertex result;
            result.position = Vector3(vert.x, height, vert.y);            
            c = 0.5f;
            result.colour = Vector3(c, c, c);            
           // printf("%f, %f, %f\n", texX, texY, texZ);
            result.textureCoords = Vector3(texX * scaleX, texY, texZ * scaleZ);
            result.topTextureCoords = Vector2(texX, texZ);
            result.normal = Vector3(0.0f, 0.0f, 0.0f);
            return result;
        }

        gpc_polygon GenerateRectangle(float x, float y, float width, float height)
        {
            gpc_polygon polygon = gpc_create_polygon(4);
            polygon.contour->vertex[3].x = x;
            polygon.contour->vertex[3].y = y;

            polygon.contour->vertex[2].x = x + width;
            polygon.contour->vertex[2].y = y;

            polygon.contour->vertex[1].x = x + width;
            polygon.contour->vertex[1].y = y + height;

            polygon.contour->vertex[0].x = x;
            polygon.contour->vertex[0].y = y + height;

            //polygon.contour->vertex[4].x = x;
           // polygon.contour->vertex[4].y = y;            

            return polygon;
        }
	
        gpc_polygon GenerateRoundedRectangle(float x, float y, float width, float height, float radius)
        {
                // Make the circle
                radius = Min(radius, Min(width, height) / 2);
                gpc_polygon circle = GenerateNGon(x + radius, y + radius, radius, 40, 0.0f, 0.0f);
                

                // Make the polygon that fills in the gaps
                //    width
                //   o------------o                  o = quarter of circle
                //   |            | height
                //   o------------o
                gpc_polygon polygon = gpc_create_polygon(8);
                
                polygon.contour->vertex[0].x = x + radius;
                polygon.contour->vertex[0].y = y;

                polygon.contour->vertex[1].x = x + width - radius;
                polygon.contour->vertex[1].y = y;
                
                polygon.contour->vertex[2].x = x + width;
                polygon.contour->vertex[2].y = y + radius;

                polygon.contour->vertex[3].x = x + width;
                polygon.contour->vertex[3].y = y + height - radius;

                polygon.contour->vertex[4].x = x + width - radius;
                polygon.contour->vertex[4].y = y + height;

                polygon.contour->vertex[5].x = x + radius;
                polygon.contour->vertex[5].y = y + height;


                polygon.contour->vertex[6].x = x;
                polygon.contour->vertex[6].y = y + height - radius;

                polygon.contour->vertex[7].x = x;
                polygon.contour->vertex[7].y = y + radius;

               // polygon.contour->vertex[8].x = x + radius;
              //  polygon.contour->vertex[8].y = y;

                // Combine with the four corners
                polygon = MergePolygons(&polygon, &circle, true, false);

                TranslatePolygon(circle, width - radius - radius, 0.0f);
                polygon = MergePolygons(&polygon, &circle, true, false);

                TranslatePolygon(circle, 0.0f, height - radius - radius);
                polygon = MergePolygons(&polygon, &circle, true, false);

                TranslatePolygon(circle, -width + radius + radius, 0.0f);
                polygon = MergePolygons(&polygon, &circle, true, false);

              //  polygon.hole[0] = 2;

                return polygon;
        }          

        inline gpc_polygon GenRandomRectangle(float x, float y, float width, float height, float radius)
        {
            if (radius >= 1.0f && Random::Rnd() < 0.5)
                return GenerateRoundedRectangle(x, y, width, height, radius);
            return GenerateRectangle(x, y, width, height);
        }

        inline gpc_polygon GenRandomRectangle(float x, float y, float width, float height, float radius, bool &wasRounded)
        {            
            if (radius >= 1.0f && Random::Rnd() < 0.5)
            {
                wasRounded = true;
                return GenerateRoundedRectangle(x, y, width, height, radius);
            }
            wasRounded = false;
            return GenerateRectangle(x, y, width, height);
        }

        void GenRectWithCutouts(const BuildingInfo &info, float x, float y, float width, float height, float radius, int shapeIndex, gpc_polygon* &outPolygons, int &numPolygons)
        {                       
            int numX, numZ;
            float sizeX, sizeZ;
            float extraX, extraY;            

            if (info.allowCutouts)
            {
                numX = info.cutOuts[0] + 1;
                numZ = info.cutOuts[1];
                sizeX = info.cutOutSize[0];
                sizeZ = info.cutOutSize[1];
            }
            else
            {
                numX = numZ = 0;
                sizeX = sizeZ = 0.0f;
            }

            if (numX == 0)
            {
                sizeX = 0.0f;
            }
            if (numZ == 0)
            {
                sizeZ = 0.0f;
            }

            if (sizeX < 1.0f)
            {
                numX = 0;
                sizeX = 0.0f;
            }
            if (sizeZ < 1.0f)
            {
                numZ = 0;
                sizeZ = 0.0f;
            }

            numPolygons = 2 * numX + 1 + 2 * numZ;
            outPolygons = new gpc_polygon[numPolygons];

            int currentPoly = 0;
            bool rounded;
            gpc_polygon rectangle = GenRandomRectangle(x + sizeZ, y + sizeX, width - 2 * sizeZ, height - 2 * sizeX, radius, rounded); // GenRandomRectangle(x, y, width, height, radius);
            gpc_polygon cutout;

            if (!rounded)
            {
                radius = 0.0f;
            }
            
         /*   if (sizeX > 0)
            {
                cutout = GenerateRectangle(x, y, width, sizeX);
                rectangle = SubtractPolygons(&rectangle, &cutout, true, true);
                cutout = GenerateRectangle(x, height - sizeX + y, width, sizeX);
                rectangle = SubtractPolygons(&rectangle, &cutout, true, true);                
            }

            if (sizeZ > 0)
            {
                cutout = GenerateRectangle(x, y, sizeZ, height);
                rectangle = SubtractPolygons(&rectangle, &cutout, true, true);
                cutout = GenerateRectangle(x + width - sizeZ, y, sizeZ, height);
                rectangle = SubtractPolygons(&rectangle, &cutout, true, true);
            }*/

            outPolygons[currentPoly++] = rectangle;//GenRandomRectangle(x + sizeZ, y + sizeX, width - sizeZ - sizeZ, height - sizeX - sizeX, radius);

            // Exit early
            if (numX == 0 && numZ == 0 )
            {
                return;
            }

            int positional = (Random::Rnd() > 0.5) ? 1 : -1;

            float offset = (width - sizeX * numX - sizeZ - sizeZ - radius - radius) / (numX + positional);
            float start = (positional == 1) ? offset : 0.0f;
            start += sizeZ + radius;
            float current = start + x;            

            // Make the extras
            for (int i = 0; i < numX; i++)
            {                
                outPolygons[currentPoly++] = GenerateRectangle(current, y, sizeX, sizeX);
                outPolygons[currentPoly++] = GenerateRectangle(current, height - sizeX + y, sizeX, sizeX);
                current += offset + sizeX;                
            }

            offset = (height - sizeZ * numZ - sizeX - sizeX - radius - radius) / (numZ + positional);
            start = (positional == 1) ? offset : 0.0f;
            start += sizeX + radius;
            current = start + y;
            for (int i = 0; i < numZ; i++)
            {
                outPolygons[currentPoly++] = GenerateRectangle(x, current, sizeZ, sizeZ);
                outPolygons[currentPoly++] = GenerateRectangle(width - sizeZ + x, current, sizeZ, sizeZ);
                current += offset + sizeZ;
            }

         /*   FILE* f = fopen("result.txt", "w");
           gpc_write_polygon(f, 0, &rectangle);
           fclose(f);*/

            return;
        }

        void GenerateGeomerty(const BuildingInfo &info, gpc_polygon polygon, float bottom, float top, unsigned int &indexOffset, std::vector<BuildingVertex> &verticies, std::vector<unsigned int> &indices)
        {
            unsigned int amt = 0;
            
            for (int j = 0; j < polygon.num_contours; j++)
            {                
                float maxX = 0, minX = info.width;
                float maxZ = 0, minZ = info.depth;

                // Do the indicies and vertices
                int start = verticies.size();

                for (int i = 0; i < polygon.contour[j].num_vertices; i++)
                {
                    gpc_vertex vert = polygon.contour[j].vertex[i];
                    maxX = Max(maxX, (float)vert.x);
                    minX = Min(minX, (float)vert.x);
                    maxZ = Max(maxZ, (float)vert.y);
                    minZ = Min(minZ, (float)vert.y);
                }

                float width = maxX - minX;
                float depth = maxZ - minZ;

                for (int i = 0; i < polygon.contour[j].num_vertices; i++)
                {
                    gpc_vertex vert = polygon.contour[j].vertex[i];
                 //   maxX = Max(maxX, (float)vert.x);
                 //   minX = Min(minX, (float)vert.x);
                 //   maxZ = Max(maxZ, (float)vert.y);
                  //  minZ = Min(minZ, (float)vert.y);

                    int scaleX, scaleY, scaleZ;
                    scaleX = Max((int)(width / 5), 1);
                    scaleY = Max((int)(top / 5), 0);
                    scaleZ = Max((int)(depth / 5), 1);

                    verticies.push_back(GetBuildingVertex(vert, bottom, Random::Rndf(), (vert.x - minX) / width, 1.0f * scaleY, (vert.y - minZ) / depth, scaleX, scaleZ));
                    verticies.push_back(GetBuildingVertex(vert, top, Random::Rndf(), (vert.x - minX) / width, 0.0f, (vert.y - minZ) / depth, scaleX, scaleZ));
                                        
                        // 	2*i + 1	|     |  2*i + 3
                        // 		    | \   |
                        // 		    |  \  |
                        // 		    |   \ |
                        // 2*i	    |    \| 2*i + 2

                    indices.push_back ((2 * i) % (polygon.contour[j].num_vertices * 2) + indexOffset);
                    indices.push_back((2 * i + 1) % (polygon.contour[j].num_vertices * 2) + indexOffset);
                    indices.push_back((2 * i + 2) % (polygon.contour[j].num_vertices * 2) + indexOffset);

                    indices.push_back((2 * i + 3) % (polygon.contour[j].num_vertices * 2) + indexOffset);
                    indices.push_back((2 * i + 1) % (polygon.contour[j].num_vertices * 2) + indexOffset);
                    indices.push_back((2 * i + 2) % (polygon.contour[j].num_vertices * 2) + indexOffset);

                    indices.push_back((2 * i + 1) % (polygon.contour[j].num_vertices * 2) + indexOffset);
                    indices.push_back((2 * i + 3) % (polygon.contour[j].num_vertices * 2) + indexOffset);
                    indices.push_back(polygon.contour[j].num_vertices * 2 + indexOffset);
                    amt += 2;		   		    
                }

                // Insert middle point
                BuildingVertex middleVert;
                middleVert.position = Vector3((maxX + minX) / 2.0f, top, (maxZ + minZ) / 2.0f);
                middleVert.colour = Vector3(0.2f, 0.2f, 0.2f);            
                middleVert.textureCoords = Vector3(0.5f, 0.5f, 0.5f);
                middleVert.topTextureCoords = Vector2(0.5f, 0.5f);
                middleVert.normal = Vector3(0, 1, 0);

                // Make the normals
                for (int i = 0; i < polygon.contour[j].num_vertices; i++)
                {
                    BuildingVertex* v1, *v2, *v3, *v4;
                    v1 = verticies.data() + start + ((2 * i) % (polygon.contour[j].num_vertices * 2));
                    v2 = verticies.data() + start + ((2 * i + 1) % (polygon.contour[j].num_vertices * 2));
                    v3 = verticies.data() + start + ((2 * i + 2) % (polygon.contour[j].num_vertices * 2));
                    v4 = verticies.data() + start + ((2 * i + 3) % (polygon.contour[j].num_vertices * 2));

                    Vector3 faceNormal1 = GetTriangleNormal(v1->position, v2->position, v3->position);
                    Vector3 faceNormal2 = GetTriangleNormal(v4->position, v3->position, v2->position);
                    Vector3 faceNormalTop = GetTriangleNormal(v2->position, v4->position, middleVert.position);                    

#if DEBUG
                    if (faceNormal1 != faceNormal2)
                    {
                        printf("UNEXPECTED ERROR: Face normals are different\n");
                    }
                    if (faceNormalTop != middleVert.normal && middleVert.normal != Vector3(0.0f, 0.0f, 0.0f))
                    {
                       printf("UNEXPECTED ERROR: Top Face normals are different\n");
                    }
                    if (faceNormal1 == Vector3(0.0f, 0.0f, 0.0f) || faceNormalTop == Vector3(0.0f, 0.0f, 0.0f))
                    {
                        printf("OK\n");
                    }
#endif

                    //middleVert.normal = faceNormalTop;
                    v3->normal = faceNormal1;                    
                }

                verticies.push_back(middleVert);
                indexOffset += amt + 1;
                amt = 0;              
            }                        

        }

        // Turns the floor plan into triangles...
        void GenerateBuffers(std::vector<BuildingVertex> &verticies, std::vector<unsigned int> &indices, GLuint &vertexBuffer, GLuint &indexBuffer)
        {
            vertexBuffer = Buffers::GenVertexBuffer(verticies.size() * sizeof(BuildingVertex), verticies.data());
            indexBuffer = Buffers::GenIndexBuffer(indices.size() * sizeof(unsigned int), indices.data());
        }
    }

    Building* GenerateBuilding(const BuildingInfo &info)
    {
        GLuint vertexBuffer, indexBuffer;        
        unsigned int indexOffset = 0;

        std::vector<gpc_polygon> toFree;
        std::vector<BuildingVertex> vertices;
        std::vector<unsigned int> indices;
        
        gpc_polygon finalPolygon;

        if (info.allowBetterShapes)
        {
            float radius = Min(info.width, info.depth) / (info.numSubShapes * 2);
            float heights = info.height / info.numSubShapes;
            float top = info.height;
            float bottom = info.height - heights;

            // Work out sizes of parts and stuff
            finalPolygon = Impl::GenerateNGon(info.width / 2, info.depth / 2, radius, Random::Rnd(3, 10), 0, 0);
            Impl::TranslatePolygon(finalPolygon, info.x, info.y);
            Impl::GenerateGeomerty(info, finalPolygon, bottom, top, indexOffset, vertices, indices);
            Impl::TranslatePolygon(finalPolygon, -info.x, -info.y);

            for (int i = 1; i < info.numSubShapes; i++)
            {
                top -= heights;
                bottom -= heights;

                float movX, movZ;
                if (Impl::GetRandomVertexOffset(info, finalPolygon, radius, movX, movZ))
                {
                    gpc_polygon newPoly = Impl::GenerateNGon(info.width / 2, info.depth / 2, radius, Random::Rnd(3, 10), movX, movZ);

                    gpc_polygon temp;
                    gpc_polygon_clip(GPC_UNION, &finalPolygon, &newPoly, &temp);
                    gpc_free_polygon(&newPoly);
                    gpc_free_polygon(&finalPolygon);

                    finalPolygon = temp;

                    Impl::TranslatePolygon(finalPolygon, info.x, info.y);
                    Impl::GenerateGeomerty(info, finalPolygon, bottom, top, indexOffset, vertices, indices);
                    Impl::TranslatePolygon(finalPolygon, -info.x, -info.y);
                }
                else
                {
                    // Finish up, can't make up
                    Impl::TranslatePolygon(finalPolygon, info.x, info.y);
                    Impl::GenerateGeomerty(info, finalPolygon, 0.0f, top, indexOffset, vertices, indices);
                    break;
                }
            }
            gpc_free_polygon(&finalPolygon);
        }
        else
        {

            gpc_polygon *polygons = NULL;
            int numPolys;

            // Work out sizes of parts and stuff

            Impl::GenRectWithCutouts(info, info.x, info.y, info.width, info.depth, Max(info.minRadius, Min(info.width, info.depth) / 2.0f), 0, polygons, numPolys);
             for (int i = 0; i < numPolys; i++)
            {
                  Impl::GenerateGeomerty(info, polygons[i], 0.0f, info.height, indexOffset, vertices, indices);
                   gpc_free_polygon(&polygons[i]);
            }
        }

        

        Impl::GenerateBuffers(vertices, indices, vertexBuffer, indexBuffer);
        printf("Made a building with %d vertices and %d indices\n", vertices.size(), indices.size());
        return new Building(vertexBuffer, indexBuffer, indices.size(), info.texture, vertices, indices, Rectangle3D(info.x, info.y, info.width, info.depth, info.height));
    }

    BuildingInfo CreateInfo(float x, float y, float width, float depth, float height, bool allowBetterShapes)
    {
        BuildingInfo result;
        result.x = x;
        result.y  = y;
        result.width = width;
        result.height = height;
        result.depth = depth;        
        result.maxRadius = Random::Rndf() * Min(width, depth) / 2.0f;
        result.numSubShapes = Random::Rnd(1, 6);
        printf("HMM: %d\n", result.numSubShapes);
        result.allowHoles = Random::Rnd() > 0.5;
        result.minRadius = Random::Rndf() * result.maxRadius;               
        result.allowBetterShapes = allowBetterShapes && Random::Rnd() > 0.5;

                
            result.allowCutouts = Random::Rnd() > 0.5;

            result.cutOutSize[0] = Random::Rndf() * Min(width, depth) * 0.15f;
            result.cutOutSize[1] = Random::Rndf() * Min(width, depth) * 0.15f;            
            result.cutOuts[0] = Random::Rnd() * Min(Max((int)(width / result.cutOutSize[0] / 2), 0), (int)(width / 10));
            result.cutOuts[1] = Random::Rnd() * Min(Max((int)(depth / result.cutOutSize[1] / 2), 0), (int)(depth / 10));        
        do
        {
            result.texture = Random::Rnd(0, BuildingTextureInfo::buildingTexureCount - 1);
        } while (result.texture == BuildingTextureInfo::buildingTextureRoofIndex);

        return result;
    }
    
}