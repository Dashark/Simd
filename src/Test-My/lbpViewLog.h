#ifndef _LBP_VIEW_LOG_H_
#define _LBP_VIEW_LOG_H_

#include <string>
#include <hdf5.h>

class lbpViewLog
{
private:
    /* data */
    hid_t       file, dataset;         /* file and dataset handles */
    hid_t       datatype, dataspace;   /* handles */
    hsize_t     dimsf[2];              /* dataset dimensions */
    herr_t      status;
    hid_t       type_id_;

public:
    lbpViewLog(const std::string &h5file_name, const std::string  &dataset_name, int nx, int ny, 
                hid_t type_id = H5T_NATIVE_UCHAR, H5T_order_t order = H5T_ORDER_LE);
    
    ~lbpViewLog();

    void write(const uint8_t *data);
};




#endif /* _LBP_VIEW_LOG_H_ */