/***********************************************************************
 * Software License Agreement (BSD License)
 *
 * Copyright 2008-2009  Marius Muja (mariusm@cs.ubc.ca). All rights reserved.
 * Copyright 2008-2009  David G. Lowe (lowe@cs.ubc.ca). All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************/

// C bindings have been heavily modified to account for index settings

#define FLANN_FIRST_MATCH

#include "wrapper.h"

struct FLANNParameters DEFAULT_FLANN_PARAMETERS = {FLANN_INDEX_KDTREE,
                                                   32,
                                                   0.0f,
                                                   0,
                                                   -1,
                                                   0,
                                                   4,
                                                   4,
                                                   32,
                                                   11,
                                                   FLANN_CENTERS_RANDOM,
                                                   0.2f,
                                                   0.9f,
                                                   0.01f,
                                                   0,
                                                   0.1f,
                                                   0,
                                                   0,
                                                   0,
                                                   FLANN_DIST_EUCLIDEAN,
                                                   3,
                                                   FLANN_LOG_NONE,
                                                   0};

using namespace flann;

flann::IndexParams create_parameters(const FLANNParameters* p) {
  flann::IndexParams params;

  params["algorithm"] = p->algorithm;

  params["checks"] = p->checks;
  params["cb_index"] = p->cb_index;
  params["eps"] = p->eps;

  if (p->algorithm == FLANN_INDEX_KDTREE) {
    params["trees"] = p->trees;
  }

  if (p->algorithm == FLANN_INDEX_KDTREE_SINGLE) {
    params["trees"] = p->trees;
    params["leaf_max_size"] = p->leaf_max_size;
  }

#ifdef FLANN_USE_CUDA
  if (p->algorithm == FLANN_INDEX_KDTREE_CUDA) {
    params["leaf_max_size"] = p->leaf_max_size;
  }
#endif

  if (p->algorithm == FLANN_INDEX_KMEANS) {
    params["branching"] = p->branching;
    params["iterations"] = p->iterations;
    params["centers_init"] = p->centers_init;
  }

  if (p->algorithm == FLANN_INDEX_AUTOTUNED) {
    params["target_precision"] = p->target_precision;
    params["build_weight"] = p->build_weight;
    params["memory_weight"] = p->memory_weight;
    params["sample_fraction"] = p->sample_fraction;
  }

  if (p->algorithm == FLANN_INDEX_HIERARCHICAL) {
    params["branching"] = p->branching;
    params["centers_init"] = p->centers_init;
    params["trees"] = p->trees;
    params["leaf_max_size"] = p->leaf_max_size;
  }

  if (p->algorithm == FLANN_INDEX_LSH) {
    params["table_number"] = p->table_number_;
    params["key_size"] = p->key_size_;
    params["multi_probe_level"] = p->multi_probe_level_;
  }

  params["log_level"] = p->log_level;
  params["random_seed"] = p->random_seed;

  return params;
}

flann::SearchParams create_search_params(const FLANNParameters* p) {
  flann::SearchParams params;
  params.checks = p->checks;
  params.eps = p->eps;
  params.sorted = p->sorted;
  params.max_neighbors = p->max_neighbors;
  params.cores = p->cores;

  return params;
}

void update_flann_parameters(const IndexParams& params,
                             FLANNParameters* flann_params) {
  if (has_param(params, "algorithm")) {
    flann_params->algorithm = get_param<flann_algorithm_t>(params, "algorithm");
  }
  if (has_param(params, "trees")) {
    flann_params->trees = get_param<int>(params, "trees");
  }
  if (has_param(params, "leaf_max_size")) {
    flann_params->leaf_max_size = get_param<int>(params, "leaf_max_size");
  }
  if (has_param(params, "branching")) {
    flann_params->branching = get_param<int>(params, "branching");
  }
  if (has_param(params, "iterations")) {
    flann_params->iterations = get_param<int>(params, "iterations");
  }
  if (has_param(params, "centers_init")) {
    flann_params->centers_init =
        get_param<flann_centers_init_t>(params, "centers_init");
  }
  if (has_param(params, "target_precision")) {
    flann_params->target_precision =
        get_param<float>(params, "target_precision");
  }
  if (has_param(params, "build_weight")) {
    flann_params->build_weight = get_param<float>(params, "build_weight");
  }
  if (has_param(params, "memory_weight")) {
    flann_params->memory_weight = get_param<float>(params, "memory_weight");
  }
  if (has_param(params, "sample_fraction")) {
    flann_params->sample_fraction = get_param<float>(params, "sample_fraction");
  }
  if (has_param(params, "table_number")) {
    flann_params->table_number_ =
        get_param<unsigned int>(params, "table_number");
  }
  if (has_param(params, "key_size")) {
    flann_params->key_size_ = get_param<unsigned int>(params, "key_size");
  }
  if (has_param(params, "multi_probe_level")) {
    flann_params->multi_probe_level_ =
        get_param<unsigned int>(params, "multi_probe_level");
  }
  if (has_param(params, "log_level")) {
    flann_params->log_level = get_param<flann_log_level_t>(params, "log_level");
  }
  if (has_param(params, "random_seed")) {
    flann_params->random_seed = get_param<long>(params, "random_seed");
  }
}

void init_flann_parameters(const FLANNParameters* p) {
  if (p != NULL) {
    flann::log_verbosity(p->log_level);
    if (p->random_seed > 0) {
      seed_random(p->random_seed);
    }
  }
}

template <typename Distance>
flann_index_t __flann_build_index(typename Distance::ElementType* dataset,
                                  int rows, int cols, float* speedup,
                                  FLANNParameters* flann_params,
                                  Distance d = Distance()) {
  typedef typename Distance::ElementType ElementType;
  try {
    init_flann_parameters(flann_params);
    if (flann_params == NULL) {
      throw FLANNException("The flann_params argument must be non-null");
    }
    IndexParams params = create_parameters(flann_params);
    Index<Distance>* index = new Index<Distance>(
        Matrix<ElementType>(dataset, rows, cols), params, d);
    index->buildIndex();

    if (flann_params->algorithm == FLANN_INDEX_AUTOTUNED) {
      IndexParams params = index->getParameters();
      update_flann_parameters(params, flann_params);
      SearchParams search_params =
          get_param<SearchParams>(params, "search_params");
      *speedup = get_param<float>(params, "speedup");
      flann_params->checks = search_params.checks;
      flann_params->eps = search_params.eps;
      flann_params->cb_index = get_param<float>(params, "cb_index", 0.0);
    }

    return index;
  } catch (std::runtime_error& e) {
    Logger::error("Caught exception: %s\n", e.what());
    return NULL;
  }
}

template <typename T>
flann_index_t _flann_build_index(T* dataset, int rows, int cols, float* speedup,
                                 FLANNParameters* flann_params) {
  if (flann_params->distance_type == FLANN_DIST_EUCLIDEAN) {
    return __flann_build_index<L2<T> >(dataset, rows, cols, speedup,
                                       flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_MANHATTAN) {
    return __flann_build_index<L1<T> >(dataset, rows, cols, speedup,
                                       flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_MINKOWSKI) {
    return __flann_build_index<MinkowskiDistance<T> >(
        dataset, rows, cols, speedup, flann_params,
        MinkowskiDistance<T>(flann_params->distance_order));
  } else if (flann_params->distance_type == FLANN_DIST_HIST_INTERSECT) {
    return __flann_build_index<HistIntersectionDistance<T> >(
        dataset, rows, cols, speedup, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_HELLINGER) {
    return __flann_build_index<HellingerDistance<T> >(dataset, rows, cols,
                                                      speedup, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_CHI_SQUARE) {
    return __flann_build_index<ChiSquareDistance<T> >(dataset, rows, cols,
                                                      speedup, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_KULLBACK_LEIBLER) {
    return __flann_build_index<KL_Divergence<T> >(dataset, rows, cols, speedup,
                                                  flann_params);
  } else {
    Logger::error(
        "Distance type unsupported in the C bindings, use the C++ bindings "
        "instead\n");
    return NULL;
  }
}

#define FLANN_BUILD_INDEX(T, R)                                               \
  flann_index_t flann_build_index_##T(T* dataset, int rows, int cols,         \
                                      float* speedup,                         \
                                      FLANNParameters* flann_params) {        \
    return _flann_build_index<T>(dataset, rows, cols, speedup, flann_params); \
  }

template <typename Distance>
int __flann_add_points(flann_index_t index_ptr,
                       typename Distance::ElementType* points, int rows,
                       int columns, float rebuild_threshold) {
  typedef typename Distance::ElementType ElementType;
  try {
    if (index_ptr == NULL) {
      throw FLANNException("Invalid index");
    }
    Index<Distance>* index = (Index<Distance>*)index_ptr;
    index->addPoints(Matrix<ElementType>(points, rows, columns),
                     rebuild_threshold);
    return 0;
  } catch (std::runtime_error& e) {
    Logger::error("Caught exception: %s\n", e.what());
    return -1;
  }
  return -1;
}

template <typename T>
int _flann_add_points(flann_index_t index_ptr, T* points, int rows, int columns,
                      float rebuild_threshold,
                      const FLANNParameters* flann_params) {
  if (flann_params->distance_type == FLANN_DIST_EUCLIDEAN) {
    return __flann_add_points<L2<T> >(index_ptr, points, rows, columns,
                                      rebuild_threshold);
  } else if (flann_params->distance_type == FLANN_DIST_MANHATTAN) {
    return __flann_add_points<L1<T> >(index_ptr, points, rows, columns,
                                      rebuild_threshold);
  } else if (flann_params->distance_type == FLANN_DIST_MINKOWSKI) {
    return __flann_add_points<MinkowskiDistance<T> >(
        index_ptr, points, rows, columns, rebuild_threshold);
  } else if (flann_params->distance_type == FLANN_DIST_HIST_INTERSECT) {
    return __flann_add_points<HistIntersectionDistance<T> >(
        index_ptr, points, rows, columns, rebuild_threshold);
  } else if (flann_params->distance_type == FLANN_DIST_HELLINGER) {
    return __flann_add_points<HellingerDistance<T> >(
        index_ptr, points, rows, columns, rebuild_threshold);
  } else if (flann_params->distance_type == FLANN_DIST_CHI_SQUARE) {
    return __flann_add_points<ChiSquareDistance<T> >(
        index_ptr, points, rows, columns, rebuild_threshold);
  } else if (flann_params->distance_type == FLANN_DIST_KULLBACK_LEIBLER) {
    return __flann_add_points<KL_Divergence<T> >(index_ptr, points, rows,
                                                 columns, rebuild_threshold);
  } else {
    Logger::error(
        "Distance type unsupported in the C bindings, use the C++ bindings "
        "instead\n");
    return 0;
  }
}

#define FLANN_ADD_POINTS(T, R)                                        \
  FLANN_EXPORT int flann_add_points_##T(                              \
      flann_index_t index_ptr, T* points, int rows, int columns,      \
      float rebuild_threshold, const FLANNParameters* flann_params) { \
    return _flann_add_points<T>(index_ptr, points, rows, columns,     \
                                rebuild_threshold, flann_params);     \
  }

template <typename Distance>
int __flann_remove_point(flann_index_t index_ptr, unsigned int point_id_uint) {
  size_t point_id(point_id_uint);
  try {
    if (index_ptr == NULL) {
      throw FLANNException("Invalid index");
    }
    Index<Distance>* index = (Index<Distance>*)index_ptr;
    index->removePoint(point_id);
    return 0;
  } catch (std::runtime_error& e) {
    Logger::error("Caught exception: %s\n", e.what());
    return -1;
  }
  return -1;
}

template <typename T>
int _flann_remove_point(flann_index_t index_ptr, unsigned int point_id,
                        const FLANNParameters* flann_params) {
  if (flann_params->distance_type == FLANN_DIST_EUCLIDEAN) {
    return __flann_remove_point<L2<T> >(index_ptr, point_id);
  } else if (flann_params->distance_type == FLANN_DIST_MANHATTAN) {
    return __flann_remove_point<L1<T> >(index_ptr, point_id);
  } else if (flann_params->distance_type == FLANN_DIST_MINKOWSKI) {
    return __flann_remove_point<MinkowskiDistance<T> >(index_ptr, point_id);
  } else if (flann_params->distance_type == FLANN_DIST_HIST_INTERSECT) {
    return __flann_remove_point<HistIntersectionDistance<T> >(index_ptr,
                                                              point_id);
  } else if (flann_params->distance_type == FLANN_DIST_HELLINGER) {
    return __flann_remove_point<HellingerDistance<T> >(index_ptr, point_id);
  } else if (flann_params->distance_type == FLANN_DIST_CHI_SQUARE) {
    return __flann_remove_point<ChiSquareDistance<T> >(index_ptr, point_id);
  } else if (flann_params->distance_type == FLANN_DIST_KULLBACK_LEIBLER) {
    return __flann_remove_point<KL_Divergence<T> >(index_ptr, point_id);
  } else {
    Logger::error(
        "Distance type unsupported in the C bindings, use the C++ bindings "
        "instead\n");
    return 0;
  }
}

#define FLANN_REMOVE_POINT(T, R)                                      \
  FLANN_EXPORT int flann_remove_point_##T(                            \
      flann_index_t index_ptr, unsigned int point_id,                 \
      const FLANNParameters* flann_params) {                          \
    return _flann_remove_point<T>(index_ptr, point_id, flann_params); \
  }

template <typename Distance>
typename Distance::ElementType* __flann_get_point(flann_index_t index_ptr,
                                                  unsigned int point_id_uint) {
  size_t point_id(point_id_uint);
  try {
    if (index_ptr == NULL) {
      throw FLANNException("Invalid index");
    }
    Index<Distance>* index = (Index<Distance>*)index_ptr;
    return index->getPoint(point_id);
  } catch (std::runtime_error& e) {
    Logger::error("Caught exception: %s\n", e.what());
    return NULL;
  }
}

template <typename T>
T* _flann_get_point(flann_index_t index_ptr, unsigned int point_id,
                    const FLANNParameters* flann_params) {
  if (flann_params->distance_type == FLANN_DIST_EUCLIDEAN) {
    return __flann_get_point<L2<T> >(index_ptr, point_id);
  } else if (flann_params->distance_type == FLANN_DIST_MANHATTAN) {
    return __flann_get_point<L1<T> >(index_ptr, point_id);
  } else if (flann_params->distance_type == FLANN_DIST_MINKOWSKI) {
    return __flann_get_point<MinkowskiDistance<T> >(index_ptr, point_id);
  } else if (flann_params->distance_type == FLANN_DIST_HIST_INTERSECT) {
    return __flann_get_point<HistIntersectionDistance<T> >(index_ptr, point_id);
  } else if (flann_params->distance_type == FLANN_DIST_HELLINGER) {
    return __flann_get_point<HellingerDistance<T> >(index_ptr, point_id);
  } else if (flann_params->distance_type == FLANN_DIST_CHI_SQUARE) {
    return __flann_get_point<ChiSquareDistance<T> >(index_ptr, point_id);
  } else if (flann_params->distance_type == FLANN_DIST_KULLBACK_LEIBLER) {
    return __flann_get_point<KL_Divergence<T> >(index_ptr, point_id);
  } else {
    Logger::error(
        "Distance type unsupported in the C bindings, use the C++ bindings "
        "instead\n");
    return NULL;
  }
}

#define FLANN_GET_POINT(T, R)                                                \
  FLANN_EXPORT int flann_get_point_##T(                                      \
      flann_index_t index_ptr, unsigned int point_id, T* point, int columns, \
      const FLANNParameters* flann_params) {                                 \
    T* value = _flann_get_point<T>(index_ptr, point_id, flann_params);       \
    if (value == NULL) {                                                     \
      return -1;                                                             \
    }                                                                        \
    for (int idx = 0; idx < columns; ++idx) {                                \
      point[idx] = value[idx];                                               \
    }                                                                        \
    return 0;                                                                \
  }

template <typename Distance>
unsigned int __flann_veclen(flann_index_t index_ptr) {
  try {
    if (index_ptr == NULL) {
      throw FLANNException("Invalid index");
    }
    Index<Distance>* index = (Index<Distance>*)index_ptr;
    return index->veclen();
  } catch (std::runtime_error& e) {
    Logger::error("Caught exception: %s\n", e.what());
    return 0;
  }
}

template <typename T>
unsigned int _flann_veclen(flann_index_t index_ptr,
                           const FLANNParameters* flann_params) {
  if (flann_params->distance_type == FLANN_DIST_EUCLIDEAN) {
    return __flann_veclen<L2<T> >(index_ptr);
  } else if (flann_params->distance_type == FLANN_DIST_MANHATTAN) {
    return __flann_veclen<L1<T> >(index_ptr);
  } else if (flann_params->distance_type == FLANN_DIST_MINKOWSKI) {
    return __flann_veclen<MinkowskiDistance<T> >(index_ptr);
  } else if (flann_params->distance_type == FLANN_DIST_HIST_INTERSECT) {
    return __flann_veclen<HistIntersectionDistance<T> >(index_ptr);
  } else if (flann_params->distance_type == FLANN_DIST_HELLINGER) {
    return __flann_veclen<HellingerDistance<T> >(index_ptr);
  } else if (flann_params->distance_type == FLANN_DIST_CHI_SQUARE) {
    return __flann_veclen<ChiSquareDistance<T> >(index_ptr);
  } else if (flann_params->distance_type == FLANN_DIST_KULLBACK_LEIBLER) {
    return __flann_veclen<KL_Divergence<T> >(index_ptr);
  } else {
    Logger::error(
        "Distance type unsupported in the C bindings, use the C++ bindings "
        "instead\n");
    return 0;
  }
}

#define FLANN_VECLEN(T, R)                                            \
  FLANN_EXPORT unsigned int flann_veclen_##T(                         \
      flann_index_t index_ptr, const FLANNParameters* flann_params) { \
    return _flann_veclen<T>(index_ptr, flann_params);                 \
  }

template <typename Distance>
unsigned int __flann_size(flann_index_t index_ptr) {
  try {
    if (index_ptr == NULL) {
      throw FLANNException("Invalid index");
    }
    Index<Distance>* index = (Index<Distance>*)index_ptr;
    return index->size();
  } catch (std::runtime_error& e) {
    Logger::error("Caught exception: %s\n", e.what());
    return 0;
  }
}

template <typename T>
unsigned int _flann_size(flann_index_t index_ptr,
                         const FLANNParameters* flann_params) {
  if (flann_params->distance_type == FLANN_DIST_EUCLIDEAN) {
    return __flann_size<L2<T> >(index_ptr);
  } else if (flann_params->distance_type == FLANN_DIST_MANHATTAN) {
    return __flann_size<L1<T> >(index_ptr);
  } else if (flann_params->distance_type == FLANN_DIST_MINKOWSKI) {
    return __flann_size<MinkowskiDistance<T> >(index_ptr);
  } else if (flann_params->distance_type == FLANN_DIST_HIST_INTERSECT) {
    return __flann_size<HistIntersectionDistance<T> >(index_ptr);
  } else if (flann_params->distance_type == FLANN_DIST_HELLINGER) {
    return __flann_size<HellingerDistance<T> >(index_ptr);
  } else if (flann_params->distance_type == FLANN_DIST_CHI_SQUARE) {
    return __flann_size<ChiSquareDistance<T> >(index_ptr);
  } else if (flann_params->distance_type == FLANN_DIST_KULLBACK_LEIBLER) {
    return __flann_size<KL_Divergence<T> >(index_ptr);
  } else {
    Logger::error(
        "Distance type unsupported in the C bindings, use the C++ bindings "
        "instead\n");
    return 0;
  }
}

#define FLANN_SIZE(T, R)                                              \
  FLANN_EXPORT unsigned int flann_size_##T(                           \
      flann_index_t index_ptr, const FLANNParameters* flann_params) { \
    return _flann_size<T>(index_ptr, flann_params);                   \
  }

template <typename Distance>
int __flann_used_memory(flann_index_t index_ptr) {
  try {
    if (index_ptr == NULL) {
      throw FLANNException("Invalid index");
    }
    Index<Distance>* index = (Index<Distance>*)index_ptr;
    return index->usedMemory();
  } catch (std::runtime_error& e) {
    Logger::error("Caught exception: %s\n", e.what());
    return 0;
  }
}

template <typename T>
int _flann_used_memory(flann_index_t index_ptr,
                       const FLANNParameters* flann_params) {
  if (flann_params->distance_type == FLANN_DIST_EUCLIDEAN) {
    return __flann_used_memory<L2<T> >(index_ptr);
  } else if (flann_params->distance_type == FLANN_DIST_MANHATTAN) {
    return __flann_used_memory<L1<T> >(index_ptr);
  } else if (flann_params->distance_type == FLANN_DIST_MINKOWSKI) {
    return __flann_used_memory<MinkowskiDistance<T> >(index_ptr);
  } else if (flann_params->distance_type == FLANN_DIST_HIST_INTERSECT) {
    return __flann_used_memory<HistIntersectionDistance<T> >(index_ptr);
  } else if (flann_params->distance_type == FLANN_DIST_HELLINGER) {
    return __flann_used_memory<HellingerDistance<T> >(index_ptr);
  } else if (flann_params->distance_type == FLANN_DIST_CHI_SQUARE) {
    return __flann_used_memory<ChiSquareDistance<T> >(index_ptr);
  } else if (flann_params->distance_type == FLANN_DIST_KULLBACK_LEIBLER) {
    return __flann_used_memory<KL_Divergence<T> >(index_ptr);
  } else {
    Logger::error(
        "Distance type unsupported in the C bindings, use the C++ bindings "
        "instead\n");
    return 0;
  }
}

#define FLANN_USED_MEMORY(T, R)                                       \
  FLANN_EXPORT int flann_used_memory_##T(                             \
      flann_index_t index_ptr, const FLANNParameters* flann_params) { \
    return _flann_used_memory<T>(index_ptr, flann_params);            \
  }

template <typename Distance>
int __flann_save_index(flann_index_t index_ptr, char* filename) {
  try {
    if (index_ptr == NULL) {
      throw FLANNException("Invalid index");
    }

    Index<Distance>* index = (Index<Distance>*)index_ptr;
    index->save(filename);

    return 0;
  } catch (std::runtime_error& e) {
    Logger::error("Caught exception: %s\n", e.what());
    return -1;
  }
}

template <typename T>
int _flann_save_index(flann_index_t index_ptr, char* filename,
                      const FLANNParameters* flann_params) {
  if (flann_params->distance_type == FLANN_DIST_EUCLIDEAN) {
    return __flann_save_index<L2<T> >(index_ptr, filename);
  } else if (flann_params->distance_type == FLANN_DIST_MANHATTAN) {
    return __flann_save_index<L1<T> >(index_ptr, filename);
  } else if (flann_params->distance_type == FLANN_DIST_MINKOWSKI) {
    return __flann_save_index<MinkowskiDistance<T> >(index_ptr, filename);
  } else if (flann_params->distance_type == FLANN_DIST_HIST_INTERSECT) {
    return __flann_save_index<HistIntersectionDistance<T> >(index_ptr,
                                                            filename);
  } else if (flann_params->distance_type == FLANN_DIST_HELLINGER) {
    return __flann_save_index<HellingerDistance<T> >(index_ptr, filename);
  } else if (flann_params->distance_type == FLANN_DIST_CHI_SQUARE) {
    return __flann_save_index<ChiSquareDistance<T> >(index_ptr, filename);
  } else if (flann_params->distance_type == FLANN_DIST_KULLBACK_LEIBLER) {
    return __flann_save_index<KL_Divergence<T> >(index_ptr, filename);
  } else {
    Logger::error(
        "Distance type unsupported in the C bindings, use the C++ bindings "
        "instead\n");
    return -1;
  }
}

#define FLANN_SAVE_INDEX(T, R)                                                 \
  FLANN_EXPORT int flann_save_index_##T(flann_index_t index_id,                \
                                        char* filename,                        \
                                        const FLANNParameters* flann_params) { \
    return _flann_save_index<T>(index_id, filename, flann_params);             \
  }

template <typename Distance>
flann_index_t __flann_load_index(char* filename,
                                 typename Distance::ElementType* dataset,
                                 int rows, int cols, Distance d = Distance()) {
  try {
    Index<Distance>* index = new Index<Distance>(
        Matrix<typename Distance::ElementType>(dataset, rows, cols),
        SavedIndexParams(filename), d);
    return index;
  } catch (std::runtime_error& e) {
    Logger::error("Caught exception: %s\n", e.what());
    return NULL;
  }
}

template <typename T>
flann_index_t _flann_load_index(char* filename, T* dataset, int rows, int cols,
                                const FLANNParameters* flann_params) {
  if (flann_params->distance_type == FLANN_DIST_EUCLIDEAN) {
    return __flann_load_index<L2<T> >(filename, dataset, rows, cols);
  } else if (flann_params->distance_type == FLANN_DIST_MANHATTAN) {
    return __flann_load_index<L1<T> >(filename, dataset, rows, cols);
  } else if (flann_params->distance_type == FLANN_DIST_MINKOWSKI) {
    return __flann_load_index<MinkowskiDistance<T> >(
        filename, dataset, rows, cols,
        MinkowskiDistance<T>(flann_params->distance_order));
  } else if (flann_params->distance_type == FLANN_DIST_HIST_INTERSECT) {
    return __flann_load_index<HistIntersectionDistance<T> >(filename, dataset,
                                                            rows, cols);
  } else if (flann_params->distance_type == FLANN_DIST_HELLINGER) {
    return __flann_load_index<HellingerDistance<T> >(filename, dataset, rows,
                                                     cols);
  } else if (flann_params->distance_type == FLANN_DIST_CHI_SQUARE) {
    return __flann_load_index<ChiSquareDistance<T> >(filename, dataset, rows,
                                                     cols);
  } else if (flann_params->distance_type == FLANN_DIST_KULLBACK_LEIBLER) {
    return __flann_load_index<KL_Divergence<T> >(filename, dataset, rows, cols);
  } else {
    Logger::error(
        "Distance type unsupported in the C bindings, use the C++ bindings "
        "instead\n");
    return NULL;
  }
}

#define FLANN_LOAD_INDEX(T, R)                                                \
  FLANN_EXPORT flann_index_t flann_load_index_##T(                            \
      char* filename, T* dataset, int rows, int cols,                         \
      const FLANNParameters* flann_params) {                                  \
    return _flann_load_index<T>(filename, dataset, rows, cols, flann_params); \
  }

template <typename Distance>
int __flann_find_nearest_neighbors(typename Distance::ElementType* dataset,
                                   int rows, int cols,
                                   typename Distance::ElementType* testset,
                                   int tcount, int* result,
                                   typename Distance::ResultType* dists, int nn,
                                   const FLANNParameters* flann_params,
                                   Distance d = Distance()) {
  typedef typename Distance::ElementType ElementType;
  typedef typename Distance::ResultType DistanceType;
  try {
    init_flann_parameters(flann_params);

    IndexParams params = create_parameters(flann_params);
    Index<Distance>* index = new Index<Distance>(
        Matrix<ElementType>(dataset, rows, cols), params, d);
    index->buildIndex();
    Matrix<int> m_indices(result, tcount, nn);
    Matrix<DistanceType> m_dists(dists, tcount, nn);
    SearchParams search_params = create_search_params(flann_params);
    index->knnSearch(Matrix<ElementType>(testset, tcount, index->veclen()),
                     m_indices, m_dists, nn, search_params);
    delete index;
    return 0;
  } catch (std::runtime_error& e) {
    Logger::error("Caught exception: %s\n", e.what());
    return -1;
  }

  return -1;
}

template <typename T, typename R>
int _flann_find_nearest_neighbors(T* dataset, int rows, int cols, T* testset,
                                  int tcount, int* result, R* dists, int nn,
                                  const FLANNParameters* flann_params) {
  if (flann_params->distance_type == FLANN_DIST_EUCLIDEAN) {
    return __flann_find_nearest_neighbors<L2<T> >(
        dataset, rows, cols, testset, tcount, result, dists, nn, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_MANHATTAN) {
    return __flann_find_nearest_neighbors<L1<T> >(
        dataset, rows, cols, testset, tcount, result, dists, nn, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_MINKOWSKI) {
    return __flann_find_nearest_neighbors<MinkowskiDistance<T> >(
        dataset, rows, cols, testset, tcount, result, dists, nn, flann_params,
        MinkowskiDistance<T>(flann_params->distance_order));
  } else if (flann_params->distance_type == FLANN_DIST_HIST_INTERSECT) {
    return __flann_find_nearest_neighbors<HistIntersectionDistance<T> >(
        dataset, rows, cols, testset, tcount, result, dists, nn, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_HELLINGER) {
    return __flann_find_nearest_neighbors<HellingerDistance<T> >(
        dataset, rows, cols, testset, tcount, result, dists, nn, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_CHI_SQUARE) {
    return __flann_find_nearest_neighbors<ChiSquareDistance<T> >(
        dataset, rows, cols, testset, tcount, result, dists, nn, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_KULLBACK_LEIBLER) {
    return __flann_find_nearest_neighbors<KL_Divergence<T> >(
        dataset, rows, cols, testset, tcount, result, dists, nn, flann_params);
  } else {
    Logger::error(
        "Distance type unsupported in the C bindings, use the C++ bindings "
        "instead\n");
    return -1;
  }
}

#define FLANN_FIND_NEAREST_NEIGHBORS(T, R)                                    \
  FLANN_EXPORT int flann_find_nearest_neighbors_##T(                          \
      T* dataset, int rows, int cols, T* testset, int trows, int* indices,    \
      R* dists, int nn, const FLANNParameters* flann_params) {                \
    return _flann_find_nearest_neighbors(dataset, rows, cols, testset, trows, \
                                         indices, dists, nn, flann_params);   \
  }

template <typename Distance>
int __flann_find_nearest_neighbors_index(
    flann_index_t index_ptr, typename Distance::ElementType* testset,
    int tcount, int* result, typename Distance::ResultType* dists, int nn,
    const FLANNParameters* flann_params) {
  typedef typename Distance::ElementType ElementType;
  typedef typename Distance::ResultType DistanceType;

  try {
    init_flann_parameters(flann_params);
    if (index_ptr == NULL) {
      throw FLANNException("Invalid index");
    }
    Index<Distance>* index = (Index<Distance>*)index_ptr;

    Matrix<int> m_indices(result, tcount, nn);
    Matrix<DistanceType> m_dists(dists, tcount, nn);

    SearchParams search_params = create_search_params(flann_params);
    index->knnSearch(Matrix<ElementType>(testset, tcount, index->veclen()),
                     m_indices, m_dists, nn, search_params);

    return 0;
  } catch (std::runtime_error& e) {
    Logger::error("Caught exception: %s\n", e.what());
    return -1;
  }

  return -1;
}

template <typename T, typename R>
int _flann_find_nearest_neighbors_index(flann_index_t index_ptr, T* testset,
                                        int tcount, int* result, R* dists,
                                        int nn,
                                        const FLANNParameters* flann_params) {
  if (flann_params->distance_type == FLANN_DIST_EUCLIDEAN) {
    return __flann_find_nearest_neighbors_index<L2<T> >(
        index_ptr, testset, tcount, result, dists, nn, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_MANHATTAN) {
    return __flann_find_nearest_neighbors_index<L1<T> >(
        index_ptr, testset, tcount, result, dists, nn, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_MINKOWSKI) {
    return __flann_find_nearest_neighbors_index<MinkowskiDistance<T> >(
        index_ptr, testset, tcount, result, dists, nn, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_HIST_INTERSECT) {
    return __flann_find_nearest_neighbors_index<HistIntersectionDistance<T> >(
        index_ptr, testset, tcount, result, dists, nn, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_HELLINGER) {
    return __flann_find_nearest_neighbors_index<HellingerDistance<T> >(
        index_ptr, testset, tcount, result, dists, nn, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_CHI_SQUARE) {
    return __flann_find_nearest_neighbors_index<ChiSquareDistance<T> >(
        index_ptr, testset, tcount, result, dists, nn, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_KULLBACK_LEIBLER) {
    return __flann_find_nearest_neighbors_index<KL_Divergence<T> >(
        index_ptr, testset, tcount, result, dists, nn, flann_params);
  } else {
    Logger::error(
        "Distance type unsupported in the C bindings, use the C++ bindings "
        "instead\n");
    return -1;
  }
}

#define FLANN_FIND_NEAREST_NEIGHBORS_INDEX(T, R)                             \
  FLANN_EXPORT int flann_find_nearest_neighbors_index_##T(                   \
      flann_index_t index_id, T* testset, int trows, int* indices, R* dists, \
      int nn, const FLANNParameters* flann_params) {                         \
    return _flann_find_nearest_neighbors_index(                              \
        index_id, testset, trows, indices, dists, nn, flann_params);         \
  }

template <typename Distance>
int __flann_radius_search(flann_index_t index_ptr,
                          typename Distance::ElementType* query, int* indices,
                          typename Distance::ResultType* dists, int max_nn,
                          float radius, const FLANNParameters* flann_params) {
  typedef typename Distance::ElementType ElementType;
  typedef typename Distance::ResultType DistanceType;

  try {
    init_flann_parameters(flann_params);
    if (index_ptr == NULL) {
      throw FLANNException("Invalid index");
    }
    Index<Distance>* index = (Index<Distance>*)index_ptr;

    Matrix<int> m_indices(indices, 1, max_nn);
    Matrix<DistanceType> m_dists(dists, 1, max_nn);
    SearchParams search_params = create_search_params(flann_params);
    int count =
        index->radiusSearch(Matrix<ElementType>(query, 1, index->veclen()),
                            m_indices, m_dists, radius, search_params);

    return count;
  } catch (std::runtime_error& e) {
    Logger::error("Caught exception: %s\n", e.what());
    return -1;
  }
}

template <typename T, typename R>
int _flann_radius_search(flann_index_t index_ptr, T* query, int* indices,
                         R* dists, int max_nn, float radius,
                         const FLANNParameters* flann_params) {
  if (flann_params->distance_type == FLANN_DIST_EUCLIDEAN) {
    return __flann_radius_search<L2<T> >(index_ptr, query, indices, dists,
                                         max_nn, radius, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_MANHATTAN) {
    return __flann_radius_search<L1<T> >(index_ptr, query, indices, dists,
                                         max_nn, radius, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_MINKOWSKI) {
    return __flann_radius_search<MinkowskiDistance<T> >(
        index_ptr, query, indices, dists, max_nn, radius, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_HIST_INTERSECT) {
    return __flann_radius_search<HistIntersectionDistance<T> >(
        index_ptr, query, indices, dists, max_nn, radius, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_HELLINGER) {
    return __flann_radius_search<HellingerDistance<T> >(
        index_ptr, query, indices, dists, max_nn, radius, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_CHI_SQUARE) {
    return __flann_radius_search<ChiSquareDistance<T> >(
        index_ptr, query, indices, dists, max_nn, radius, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_KULLBACK_LEIBLER) {
    return __flann_radius_search<KL_Divergence<T> >(
        index_ptr, query, indices, dists, max_nn, radius, flann_params);
  } else {
    Logger::error(
        "Distance type unsupported in the C bindings, use the C++ bindings "
        "instead\n");
    return -1;
  }
}

#define FLANN_RADIUS_SEARCH(T, R)                                            \
  FLANN_EXPORT int flann_radius_search_##T(                                  \
      flann_index_t index_ptr, T* query, int* indices, R* dists, int max_nn, \
      float radius, const FLANNParameters* flann_params) {                   \
    return _flann_radius_search(index_ptr, query, indices, dists, max_nn,    \
                                radius, flann_params);                       \
  }

template <typename Distance>
int __flann_free_index(flann_index_t index_ptr,
                       const FLANNParameters* flann_params) {
  try {
    init_flann_parameters(flann_params);
    if (index_ptr == NULL) {
      throw FLANNException("Invalid index");
    }
    Index<Distance>* index = (Index<Distance>*)index_ptr;
    delete index;

    return 0;
  } catch (std::runtime_error& e) {
    Logger::error("Caught exception: %s\n", e.what());
    return -1;
  }
}

template <typename T>
int _flann_free_index(flann_index_t index_ptr,
                      const FLANNParameters* flann_params) {
  if (flann_params->distance_type == FLANN_DIST_EUCLIDEAN) {
    return __flann_free_index<L2<T> >(index_ptr, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_MANHATTAN) {
    return __flann_free_index<L1<T> >(index_ptr, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_MINKOWSKI) {
    return __flann_free_index<MinkowskiDistance<T> >(index_ptr, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_HIST_INTERSECT) {
    return __flann_free_index<HistIntersectionDistance<T> >(index_ptr,
                                                            flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_HELLINGER) {
    return __flann_free_index<HellingerDistance<T> >(index_ptr, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_CHI_SQUARE) {
    return __flann_free_index<ChiSquareDistance<T> >(index_ptr, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_KULLBACK_LEIBLER) {
    return __flann_free_index<KL_Divergence<T> >(index_ptr, flann_params);
  } else {
    Logger::error(
        "Distance type unsupported in the C bindings, use the C++ bindings "
        "instead\n");
    return -1;
  }
}

#define FLANN_FREE_INDEX(T, R)                                                 \
  FLANN_EXPORT int flann_free_index_##T(flann_index_t index_id,                \
                                        const FLANNParameters* flann_params) { \
    return _flann_free_index<T>(index_id, flann_params);                       \
  }

template <typename Distance>
int __flann_compute_cluster_centers(typename Distance::ElementType* dataset,
                                    int rows, int cols, int clusters,
                                    typename Distance::ResultType* result,
                                    const FLANNParameters* flann_params,
                                    Distance d = Distance()) {
  typedef typename Distance::ElementType ElementType;
  typedef typename Distance::ResultType DistanceType;

  try {
    init_flann_parameters(flann_params);

    Matrix<ElementType> inputData(dataset, rows, cols);
    KMeansIndexParams params(flann_params->branching, flann_params->iterations,
                             flann_params->centers_init,
                             flann_params->cb_index);
    Matrix<DistanceType> centers(result, clusters, cols);
    int clusterNum =
        hierarchicalClustering<Distance>(inputData, centers, params, d);

    return clusterNum;
  } catch (std::runtime_error& e) {
    Logger::error("Caught exception: %s\n", e.what());
    return -1;
  }
}

template <typename T, typename R>
int _flann_compute_cluster_centers(T* dataset, int rows, int cols, int clusters,
                                   R* result,
                                   const FLANNParameters* flann_params) {
  if (flann_params->distance_type == FLANN_DIST_EUCLIDEAN) {
    return __flann_compute_cluster_centers<L2<T> >(
        dataset, rows, cols, clusters, result, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_MANHATTAN) {
    return __flann_compute_cluster_centers<L1<T> >(
        dataset, rows, cols, clusters, result, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_MINKOWSKI) {
    return __flann_compute_cluster_centers<MinkowskiDistance<T> >(
        dataset, rows, cols, clusters, result, flann_params,
        MinkowskiDistance<T>(flann_params->distance_order));
  } else if (flann_params->distance_type == FLANN_DIST_HIST_INTERSECT) {
    return __flann_compute_cluster_centers<HistIntersectionDistance<T> >(
        dataset, rows, cols, clusters, result, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_HELLINGER) {
    return __flann_compute_cluster_centers<HellingerDistance<T> >(
        dataset, rows, cols, clusters, result, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_CHI_SQUARE) {
    return __flann_compute_cluster_centers<ChiSquareDistance<T> >(
        dataset, rows, cols, clusters, result, flann_params);
  } else if (flann_params->distance_type == FLANN_DIST_KULLBACK_LEIBLER) {
    return __flann_compute_cluster_centers<KL_Divergence<T> >(
        dataset, rows, cols, clusters, result, flann_params);
  } else {
    Logger::error(
        "Distance type unsupported in the C bindings, use the C++ bindings "
        "instead\n");
    return -1;
  }
}

#define FLANN_COMPUTE_CLUSTER_CENTERS(T, R)                              \
  FLANN_EXPORT int flann_compute_cluster_centers_##T(                    \
      T* dataset, int rows, int cols, int clusters, R* result,           \
      const FLANNParameters* flann_params) {                             \
    return _flann_compute_cluster_centers(dataset, rows, cols, clusters, \
                                          result, flann_params);         \
  }

#define FLANN_IMPL(T, R)                   \
  FLANN_BUILD_INDEX(T, R)                  \
  FLANN_ADD_POINTS(T, R)                   \
  FLANN_REMOVE_POINT(T, R)                 \
  FLANN_GET_POINT(T, R)                    \
  FLANN_VECLEN(T, R)                       \
  FLANN_SIZE(T, R)                         \
  FLANN_USED_MEMORY(T, R)                  \
  FLANN_SAVE_INDEX(T, R)                   \
  FLANN_LOAD_INDEX(T, R)                   \
  FLANN_FIND_NEAREST_NEIGHBORS(T, R)       \
  FLANN_FIND_NEAREST_NEIGHBORS_INDEX(T, R) \
  FLANN_RADIUS_SEARCH(T, R)                \
  FLANN_FREE_INDEX(T, R)                   \
  FLANN_COMPUTE_CLUSTER_CENTERS(T, R)

FLANN_IMPL(float, float)
FLANN_IMPL(double, double)
FLANN_IMPL(char, float)
FLANN_IMPL(int16_t, float)
FLANN_IMPL(int32_t, float)
FLANN_IMPL(uint8_t, float)
FLANN_IMPL(uint16_t, float)
// FLANN_IMPL(uint32_t, float)

#undef FLANN_BUILD_INDEX
#undef FLANN_ADD_POINTS
#undef FLANN_REMOVE_POINT
#undef FLANN_GET_POINT
#undef FLANN_VECLEN
#undef FLANN_SIZE
#undef FLANN_USED_MEMORY
#undef FLANN_SAVE_INDEX
#undef FLANN_LOAD_INDEX
#undef FLANN_FIND_NEAREST_NEIGHBORS
#undef FLANN_FIND_NEAREST_NEIGHBORS_INDEX
#undef FLANN_RADIUS_SEARCH
#undef FLANN_FREE_INDEX
#undef FLANN_COMPUTE_CLUSTER_CENTERS
#undef FLANN_IMPL
