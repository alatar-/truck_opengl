#include "mesh.h"
#include "bone_vertex.h"
#include <vector>

using namespace std;

mesh_t::mesh_t (const aiMesh *paiMesh, material_t *in_material) {
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
	
	{
		unsigned size = indices.size();
		pos_data = (float*)calloc(sizeof(float) * 3, size);
		norm_data = (float*)calloc(sizeof(float) * 3, size);
		tex_coords = (float*)calloc(sizeof(float) * 2, size);
	}
	
	
	vertices.reserve(paiMesh->mNumVertices);
	
	{
		for (unsigned i = 0, len = paiMesh->mNumVertices; i < len; ++i) {
			const aiVector3D* pos = &(paiMesh->mVertices[i]);
			const aiVector3D* norm = &(paiMesh->mNormals[i]);
			vertex_3d v_pos(pos->x, pos->y, pos->z);
			
			if (i == 0) {
				cords_min.x = v_pos.x;
				cords_min.y = v_pos.z;
				cords_max.x = v_pos.x;
				cords_max.y = v_pos.z;
			} else {
				if(v_pos.x < cords_min.x) {
					cords_min.x = v_pos.x;
				}
				if(v_pos.z < cords_min.y) {
					cords_min.y = v_pos.z;
				}
				if(v_pos.x > cords_max.x) {
					cords_max.x = v_pos.x;
				}
				if(v_pos.z > cords_min.y) {
					cords_max.y = v_pos.z;
				}
			}
			
			vertices.push_back(bone_vertex_t(v_pos, vertex_3d(norm->x, norm->y, norm->z)));
		}
	}
	
	set_vertices_data();
	
	for (unsigned i = 0, v = indices[i], len = indices.size()
		;	i < len
		;	++i, v = indices[i]) {
		const aiVector3D* tex_coord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][v]) : &Zero3D;
		tex_coords[i * 2] = tex_coord->x;
		tex_coords[i * 2 + 1] = tex_coord->y;
	}
}

mesh_t::~mesh_t() {
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

void mesh_t::draw (bool use_mv, bool apply_material, glm::mat4 V) {
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


void mesh_t::set_vertices_data() {
// 	printf("mesh_t::set_vertices_data(%p)> recalced\n", this);
	for (unsigned i = 0, v = indices[i], len = indices.size()
		;	i < len
		;	++i, v = indices[i]) {
		vertex_3d pos = vertices[v].get_pos()
			,	norm = vertices[v].get_norm();
		pos_data[i * 3] = pos.x;
		pos_data[i * 3 + 1] = pos.y;
		pos_data[i * 3 + 2] = pos.z;
		
		norm_data[i * 3] = norm.x;
		norm_data[i * 3 + 1] = norm.y;
		norm_data[i * 3 + 2] = norm.z;
	}
// 	printf("mesh_t::set_vertices_data(%p)> set\n", this);
}

bone_vertex_t& mesh_t::operator[] (unsigned i) {
	return vertices[i];
}

unsigned mesh_t::vertices_size() {
	return vertices.size();
}

void mesh_t::print() {
	for (unsigned i = 0, len = vertices.size(); i < len; ++i) {
		vertex_3d v = vertices[i].get_pos();
		printf("v[%u] = (%.2f, %.2f, %.2f)\n", i, v.x, v.y, v.z);
	}
}

void mesh_t::set_mv_matrix (glm::mat4 in_MV) {
	MV = in_MV;
	
	if (test_visibility()) {
		material->set_marker(true);
	}
}

bool mesh_t::test_visibility() {
	return true;
}

bool mesh_t::test_intersection (vertex_3d pos0, vertex_3d pos1) {
	for (unsigned i = 0, ilen = indices.size(); i < ilen; i += 9) {
		if (check_intersect_tri(vertex_3d(pos_data + i), vertex_3d(pos_data + i + 3), vertex_3d(pos_data + i + 6), pos0, pos1)) {
			return true;
		}
	}
	return false;
}

vertex_2d mesh_t::get_cords_min() {
	return cords_min;
}

vertex_2d mesh_t::get_cords_max() {
	return cords_max;
}
