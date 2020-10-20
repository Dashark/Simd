#include <assert.h>

#include "lbpViewLog.h"

lbpViewLog::lbpViewLog(const std::string &h5file_name, const std::string  &dataset_name, int nx, int ny, hid_t type_id, H5T_order_t order) {
    /*
     * Create a new file using H5F_ACC_TRUNC access,
     * default file creation properties, and default file
     * access properties.
     */
    file = H5Fcreate(h5file_name.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    /*
     * Describe the size of the array and create the data space for fixed
     * size dataset.
     */
    dimsf[0] = nx;
    dimsf[1] = ny;
    dataspace = H5Screate_simple(2, dimsf, NULL);

    /*
     * Define datatype for the data in the file.
     * We will store little endian INT numbers.
     */
    type_id_ = type_id;
    datatype = H5Tcopy(type_id);
    status = H5Tset_order(datatype, order);
    assert(status >= 0);

    /*
     * Create a new dataset within the file using defined dataspace and
     * datatype and default dataset creation properties.
     */
    dataset = H5Dcreate2(file, dataset_name.c_str(), datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
}

lbpViewLog::~lbpViewLog() {
    /*
     * Close/release resources.
     */
    H5Sclose(dataspace);
    H5Tclose(datatype);
    H5Dclose(dataset);
    H5Fclose(file);
}

 void lbpViewLog::write(const uint8_t *data) {
     assert(type_id_ == H5T_NATIVE_UCHAR);
     /*
     * Write the data to the dataset using default transfer properties.
     */
     status = H5Dwrite(dataset, type_id_, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
     assert(status >= 0);
 }