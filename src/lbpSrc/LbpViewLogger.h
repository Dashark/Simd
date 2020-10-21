#ifndef _LBP_VIEW_LOGGER_H_
#define _LBP_VIEW_LOGGER_H_

#include <string>
#include <hdf5.h>

/*
 * 功能: 将View的data数据记录为HDF5文件
*/
class lbpViewLogger {
private:
    /* data */
    hid_t       file, dataset;         /* file and dataset handles */
    hid_t       datatype, dataspace;   /* handles */
    hsize_t     dimsf[2];              /* dataset dimensions */
    herr_t      status;
    hid_t       type_id_;

public:
    lbpViewLogger(const std::string &h5file_name, const std::string  &dataset_name, int width, int height, 
                hid_t type_id = H5T_NATIVE_UCHAR, H5T_order_t order = H5T_ORDER_LE);
    
    ~lbpViewLogger();

    void write(const uint8_t *data, size_t stride, size_t width, size_t height); //传入值为View的data成员(如果stride!=width,需先处理data)
};




#endif /* _LBP_VIEW_LOGGER_H_ */
