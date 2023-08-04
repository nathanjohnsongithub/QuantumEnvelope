#include <pybind11/pybind11.h>
#include <vector>
#include <algorithm>

using namespace std;

typedef vector<int> sdet_vec_t;

sdet_vec_t makeVectors(pybind11::tuple tuple){
    // initialize the vector
    sdet_vec_t vec;
    
    // Convert the tuple into a vector using .push_back which is similar to append
    for (size_t i = 0; i < tuple.size(); ++i) {
        vec.push_back(tuple[i].cast<int>());
    }
    
    return vec;
}

tuple<int, int> exc_degree_tuple(pybind11::tuple det_I_alpha, pybind11::tuple det_I_beta, pybind11::tuple det_J_alpha, pybind11::tuple det_J_beta) {
    /* 
    Create all of the vectors from the python tuples using the makeVectors method we created
    we need to put them into vectors because cant use set_symetric_differece with python tuples
    */
    sdet_vec_t det_I_alpha_vec = makeVectors(det_I_alpha);
    sdet_vec_t det_I_beta_vec = makeVectors(det_I_beta);
    sdet_vec_t det_J_alpha_vec = makeVectors(det_J_alpha);
    sdet_vec_t det_J_beta_vec = makeVectors(det_J_beta);

    // define the vectors that set symmetric difference will output to
    sdet_vec_t XOR_res_a, XOR_res_b;

    // take the symmetric difference of both alpha and beta electrons 
    set_symmetric_difference(det_I_alpha_vec.begin(), det_I_alpha_vec.end(), det_J_alpha_vec.begin(), det_J_alpha_vec.end(), back_inserter(XOR_res_a));
    set_symmetric_difference(det_I_beta_vec.begin(), det_I_beta_vec.end(), det_J_beta_vec.begin(), det_J_beta_vec.end(), back_inserter(XOR_res_b));
    
    // divide the size by 2 because they come in pairs of 2
    int ed_up = XOR_res_a.size() / 2;
    int ed_dn = XOR_res_b.size() / 2;  
    // return a tuple of the calulated values
    return make_tuple(ed_up, ed_dn);
}

// Define the module
PYBIND11_MODULE(qpx_cpp_module, m) {
    m.def("exc_degree_tuple", &exc_degree_tuple, "Compute excitation degree; the number of orbitals which differ between two |Determinants| det_I, det_J");
}

