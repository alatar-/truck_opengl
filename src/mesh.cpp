#include "mesh.h"
#include "extra_vertex.h"
#include <vector>

using namespace std;

Mesh::Mesh (const aiMesh *paiMesh, Material *in_material) : MV(1.0f) {
	material = in_material;
	material->meshes.push_back(this);
	
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
	
	for (unsigned i = 0; i < paiMesh->mNumFaces; ++i) {
		const aiFace& Face = paiMesh->mFaces[i];
		if (Face.mNumIndices == 3) {
			indices.push_back(Face.mIndices[0]);
			indices.push_back(Face.mIndices[1]);
			indices.push_back(Face.mIndices[2]);
		}
	}
	
	unsigned size = indices.size();
	pos_data = (float*)calloc(sizeof(float) * 3, size);
	norm_data = (float*)calloc(sizeof(float) * 3, size);
	tex_coords = (float*)calloc(sizeof(float) * 2, size);
	
	vertices.reserve(paiMesh->mNumVertices);
	
	for (unsigned i = 0, len = paiMesh->mNumVertices; i < len; ++i) {
		const aiVector3D* pos = &(paiMesh->mVertices[i]);
		const aiVector3D* norm = &(paiMesh->mNormals[i]);
		Vertex3D<float> v_pos(pos->x, pos->y, pos->z);
		
		if (i == 0) {
			cords_min.x = v_pos.x;
			cords_min.y = v_pos.z;
			cords_max.x = v_pos.x;
			cords_max.y = v_pos.z;
		} else {
			cords_min.x = min(cords_min.x, v_pos.x);
			cords_min.y = min(cords_min.y, v_pos.z);
			cords_max.x = max(cords_max.x, v_pos.x);
			cords_max.y = max(cords_max.y, v_pos.z);
		}
		
		vertices.push_back(ExtraVertex(v_pos, Vertex3D<float>(norm->x, norm->y, norm->z)));
	}
	
	set_vertices_data();
	
	for (unsigned i = 0, v = indices[i], len = indices.size(); i < len;	++i, v = indices[i]) {
		const aiVector3D* tex_coord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][v]) : &Zero3D;
		tex_coords[i * 2] = tex_coord->x;
		tex_coords[i * 2 + 1] = tex_coord->y;
	}
}

Mesh::~Mesh() {
	indices.clear();
	vertices.clear();
	delete pos_data;
	delete norm_data;
	delete tex_coords;
	pos_data = NULL;
	norm_data = NULL;
	tex_coords = NULL;
	material = NULL;
}

void Mesh::draw (bool use_mv, bool apply_material, glm::mat4 V) {
	if (use_mv) {
		glLoadMatrixf(glm::value_ptr(V * MV));
	}
	
	if (apply_material) {
		material->apply();
	}
	
	glVertexPointer(3,GL_FLOAT,0, pos_data);
	glNormalPointer(GL_FLOAT, 0, norm_data);
	glTexCoordPointer(2, GL_FLOAT, 0, tex_coords);
	
	glDrawArrays(GL_TRIANGLES, 0, indices.size());
}


void Mesh::set_vertices_data() {
	for (unsigned i = 0, v = indices[i], len = indices.size(); i < len;	++i, v = indices[i]) {
		Vertex3D<float> pos = vertices[v].get_pos();
        Vertex3D<float> norm = vertices[v].get_norm();
		pos_data[i * 3] = pos.x;
		pos_data[i * 3 + 1] = pos.y;
		pos_data[i * 3 + 2] = pos.z;
		
		norm_data[i * 3] = norm.x;
		norm_data[i * 3 + 1] = norm.y;
		norm_data[i * 3 + 2] = norm.z;
	}
}

ExtraVertex& Mesh::operator[] (unsigned i) {
	return vertices[i];
}

unsigned Mesh::vertices_size() {
	return vertices.size();
}

void Mesh::print() {
	for (unsigned i = 0, len = vertices.size(); i < len; ++i) {
		Vertex3D<float> v = vertices[i].get_pos();
		printf("v[%u] = (%.2f, %.2f, %.2f)\n", i, v.x, v.y, v.z);
	}
}

void Mesh::set_mv_matrix (glm::mat4 in_MV) {
	MV = in_MV;
}

void Mesh::set_material (Material *in_material) {
	material = in_material;
}

Vertex2D<float> Mesh::get_cords_min() {
	return cords_min;
}

Vertex2D<float> Mesh::get_cords_max() {
	return cords_max;
}
