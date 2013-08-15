#include "anim_mesh_model.h"

anim_mesh_model_t::anim_mesh_model_t() : anim_model_t() {
}

anim_mesh_model_t::~anim_mesh_model_t() {
	clear();
}

void anim_mesh_model_t::clear() {
	model_t::clear();
	frames.clear();
}

bool anim_mesh_model_t::load (string filename, float in_fps, unsigned in_frames) {
	bool res;
	
	filename += "/";
	res = model_t::load(filename + "001.obj");
	
	if (!res) {
		return false;
	}
	frames.push_back(vector <vector <ExtraVertex> >());
	for (unsigned i = 0, ilen = meshes.size(); i < ilen; ++i) {
		frames[0].push_back(vector <ExtraVertex>());
		frames[0][i].reserve(meshes[i]->vertices_size());
		for (unsigned j = 0, jlen = meshes[i]->vertices_size(); j < jlen; ++j) {
			mesh_t &curr_mesh = (*meshes[i]);
			frames[0][i].push_back(curr_mesh[j]);
		}
	}
	
	for (unsigned i = 2; i <= in_frames; ++i) {
		load_frame(filename, i);
	}
	
	full_time = frames.size() / in_fps;
	return true;
}

bool anim_mesh_model_t::load_frame (string filename, unsigned idx) {
	char number[11];
	sprintf(number, "%03u", idx);
	number[3] = '\0';
	filename += number;
	filename += ".obj";
	
	printf("loading: '%s'\n", filename.c_str());
	
	
	Assimp::Importer Importer;
	const aiScene* pScene = Importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	
	if (pScene) {
		frames.push_back(vector <vector <ExtraVertex> >());
		vector <vector <ExtraVertex> > &the_frame = frames.back();
		the_frame.reserve(pScene->mNumMeshes);
		
		for (unsigned i = 0, ilen = pScene->mNumMeshes; i < ilen; ++i) {
			const aiMesh* paiMesh = pScene->mMeshes[i];
			
			the_frame.push_back(vector <ExtraVertex>());
			vector <ExtraVertex> &the_mesh = the_frame.back();
			the_mesh.reserve(paiMesh->mNumVertices);
			
			for (unsigned j = 0, jlen = paiMesh->mNumVertices; j < jlen; ++j) {
				const aiVector3D* pos = &(paiMesh->mVertices[j]);
				const aiVector3D* norm = &(paiMesh->mNormals[j]);
				
				the_mesh.push_back(ExtraVertex(Vertex3D<float>pos->x, pos->y, pos->z), Vertex3D<float>norm->x, norm->y, norm->z)));
				
// 				printf("loaded vertex> (%.2f, %.2f, %.2f) - (%.2f, %.2f, %.2f)\n", the_mesh.back().get_pos().x, the_mesh.back().get_pos().y, the_mesh.back().get_pos().z, the_mesh.back().get_norm().x, the_mesh.back().get_norm().y, the_mesh.back().get_norm().z);
// 				printf("loaded vertex> (%.2f, %.2f, %.2f) - (%.2f, %.2f, %.2f)\n", pos->x, pos->y, pos->z, norm->x, norm->y, norm->z);
			}
		}
	} else {
		fprintf(stderr, "Error parsing '%s': '%s'\n", filename.c_str(), Importer.GetErrorString());
		return false;
	}
	return true;
}

void anim_mesh_model_t::recalc_vertices() {
// 	printf("anim_mesh_model_t::recalc_vertices\n");
	for (unsigned i = 0, len = meshes.size(); i < len; ++i) {
		meshes[i]->set_vertices_data();
	}
}

void anim_mesh_model_t::prepare_vertices (float frame) {
	unsigned ith = ((unsigned)frame) % frames.size();
// 	printf("anim_mesh_model_t::prepare_vertices(%.2f)> ith=%u\n", frame, ith);
	if ((float)ith == frame) {
		
		vector <vector <ExtraVertex> > &the_frame = frames[ith];
		for (unsigned i = 0, ilen = meshes.size(); i < ilen; ++i) {
			mesh_t &the_mesh = (*meshes[i]);
			vector <ExtraVertex> &frame_mesh = the_frame[i];
			for (unsigned j = 0, jlen = the_mesh.vertices_size(); j < jlen; ++j) {
				the_mesh[j] = frame_mesh[j];
			}
// 			meshes[i]->print();
		}
		return;
	} else {
		unsigned jth = (ith + 1) % frames.size();
		frame -= ith;
		
		vector <vector <ExtraVertex> > &ith_frame = frames[ith]
			,	&jth_frame = frames[jth];
		for (unsigned i = 0, ilen = meshes.size(); i < ilen; ++i) {
			mesh_t &the_mesh = (*meshes[i]);
			vector <ExtraVertex> &ith_frame_mesh = ith_frame[i]
				,	&jth_frame_mesh = jth_frame[i];
			for (unsigned j = 0, jlen = the_mesh.vertices_size(); j < jlen; ++j) {
				// or slerp??
				the_mesh[j].set_pos(Vertex3D<float>:lerp(ith_frame_mesh[j].get_pos(), jth_frame_mesh[j].get_pos(), frame));
				the_mesh[j].set_norm(Vertex3D<float>:lerp(ith_frame_mesh[j].get_norm(), jth_frame_mesh[j].get_norm(), frame));
			}
		}
	}
}

void anim_mesh_model_t::animate (float anim_time) {
// 	printf("anim_mesh_model_t::animate(%.2f)> %.2f\n", anim_time, fmod(anim_time, full_time) / full_time * frames.size());
	if (full_time) {
		prepare_vertices(fmod(anim_time, full_time) / full_time * frames.size());
		recalc_vertices();
	}
}
