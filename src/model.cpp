#include "model.h"

model_t::model_t () {
}

bool model_t::load (string model_file) {
	clear();
	
	Assimp::Importer Importer;
	const aiScene* pScene = Importer.ReadFile(model_file.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	
	if (!parse_scene(pScene, model_file)) {
		fprintf(stderr, "Error parsing '%s': '%s'\n", model_file.c_str(), Importer.GetErrorString());
		return false;
	} else {
		return true;
	}
}

bool model_t::parse_scene (const aiScene *pScene, string &model_file) {
	if (pScene) {
		meshes.reserve(pScene->mNumMeshes);
		materials.reserve(pScene->mNumMaterials);
		
		if (!load_materials(pScene, model_file)) {
			return false;
		}
		
		for (unsigned i = 0, len = pScene->mNumMeshes; i < len; ++i) {
			const aiMesh* paiMesh = pScene->mMeshes[i];
			meshes.push_back(new mesh_t(paiMesh, materials[paiMesh->mMaterialIndex]));
		}
	} else {
		return false;
	}
	
	return true;
}


bool model_t::load_materials (const aiScene* pScene, const string& model_file) {
	// Extract the directory part from the file name
	string::size_type SlashIndex = model_file.find_last_of("/");
	string dir;
	
	if (SlashIndex == string::npos) {
		dir = ".";
	} else if (SlashIndex == 0) {
		dir = "/";
	} else {
		dir = model_file.substr(0, SlashIndex);
	}
	
	// Initialize the materials
	for (unsigned i = 0; i < pScene->mNumMaterials; ++i) {
		materials.push_back(new material_t());
		if (!materials[i]->load(pScene->mMaterials[i], dir)) {
			return false;
		}
	}
	
	return true;
}

void model_t::clear() {
	for (unsigned i = 0, len = materials.size(); i < len; ++i) {
		delete materials[i];
	}
	
	materials.clear();
	
	for (unsigned i = 0, len = meshes.size(); i < len; ++i) {
		delete meshes[i];
	}
	
	meshes.clear();
}

model_t::~model_t() {
	clear();
}

void model_t::set_mv_matrix (glm::mat4 MV) {
	for (unsigned i = 0, ilen = meshes.size(); i < ilen; ++i) {
		meshes[i]->set_mv_matrix(MV);
	}
}

void model_t::draw (bool use_mv) {
	for (unsigned i = 0, len = meshes.size(); i < len; ++i) {
		meshes[i]->draw(false, true);
	}
}

bool model_t::test_intersection (vertex_3d pos0, vertex_3d pos1) {
	for (unsigned i = 0, ilen = meshes.size(); i < ilen; ++i) {
		if (meshes[i]->test_intersection(pos0, pos1)) {
			return true;
		}
	}
	return false;
}
