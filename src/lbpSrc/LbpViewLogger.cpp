#include <assert.h>
#include <string.h>

#include "lbpSrc/LbpViewLogger.h"

lbpViewLogger::lbpViewLogger(const std::string &h5file_name, const std::string  &dataset_name, int width, int height, hid_t type_id, H5T_order_t order) {
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
    dimsf[0] = height;
    dimsf[1] = width;
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

lbpViewLogger::~lbpViewLogger() {
    /*
     * Close/release resources.
     */
    H5Sclose(dataspace);
    H5Tclose(datatype);
    H5Dclose(dataset);
    H5Fclose(file);
}

 void lbpViewLogger::write(const uint8_t *data, size_t stride, size_t width, size_t height) {
     assert(type_id_ == H5T_NATIVE_UCHAR);
     /*
     * Write the data to the dataset using default transfer properties.
     */
    if (stride == width) {
        status = H5Dwrite(dataset, type_id_, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
    } else { //data数组有空位, 需处理
        uint8_t *dest = new uint8_t[width * height](); //初始化为0

        for (size_t row = 0; row < height; row++) {
            memcpy(&dest[row*width], &data[row*stride], width);
        }

        status = H5Dwrite(dataset, type_id_, H5S_ALL, H5S_ALL, H5P_DEFAULT, dest);

        delete []dest;
    }
    
    assert(status >= 0);
 }

