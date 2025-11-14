#include "framework.h"

#if !defined(__APPLE__) && !defined(WINDOWS)
#include "assimp.h"

#define ASSIMP_USE_HUNTER
#define ASSIMP_DLL


#undef min
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"


namespace assimp
{


   bool load_obj(const void * data, memsize size, array<unsigned short> & indices, array<::floating_sequence3> & vertices, array<::floating_sequence2> & uvs, array<::floating_sequence3> & normals)
   {

      Assimp::Importer importer;

      //const aiScene * scene = importer.ReadFile(path, aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
      //const aiScene * scene = importer.ReadFile(path, 0);
      //const aiScene* scene = importer.ReadFileFromMemory(data, size, aiProcess_SortByPType);
       const aiScene* scene = importer.ReadFileFromMemory(data, size, aiProcess_FlipUVs);
      //const aiScene * scene = importer.ReadFileFromMemory(data, size, 0);
      if (!scene) {
         fprintf(stderr, "%s", importer.GetErrorString());
         getchar();
         return false;
      }

      const aiMesh * mesh = scene->mMeshes[0]; // In this simple example code we always use the 1rst mesh (in OBJ files there is often only one anyway)

      // Fill vertices positions
      vertices.reserve(mesh->mNumVertices);
      for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
         aiVector3D pos = mesh->mVertices[i];
         vertices.add(floating_sequence3(pos.x, pos.y, pos.z));
      }

      // Fill vertices texture coordinates
      uvs.reserve(mesh->mNumVertices);
      for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
         aiVector3D UVW = mesh->mTextureCoords[0][i]; // Assume only 1 set of UV coords; AssImp supports 8 UV sets.
         uvs.add(floating_sequence2(UVW.x, UVW.y));
      }

      // Fill vertices normals
      normals.reserve(mesh->mNumVertices);
      for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
         aiVector3D n = mesh->mNormals[i];
         normals.add(floating_sequence3(n.x, n.y, n.z));
      }

      // Fill face indices
      indices.reserve(3 * mesh->mNumFaces);
      for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
         // Assume the model has only triangles.
         indices.add(mesh->mFaces[i].mIndices[0]);
         indices.add(mesh->mFaces[i].mIndices[1]);
         indices.add(mesh->mFaces[i].mIndices[2]);
      }

      // The "scene" pointer will be deleted automatically by "importer"
      return true;

   }


} // namespace assimp



#endif



