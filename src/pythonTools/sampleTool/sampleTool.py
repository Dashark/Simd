#!/usr/bin/python3

import json
import os
from cv2 import cv2

slice_test = True # 是否保存切片，本代码测试用

def saveBinaryImages(full_path, gray_img):
    '''
    : 功能：参照宋的代码，在写二进制文件的前面加上图片的高度和宽度
           adaBoostApp 使用
    '''
    binary_image = open(full_path, "wb")
    h = gray_img.shape[0]
    w = gray_img.shape[1]
    binary_image.write(int(h).to_bytes(4, byteorder="little", signed=True))
    binary_image.write(int(w).to_bytes(4, byteorder="little", signed=True))
    binary_image.write(gray_img.tobytes())
    binary_image.close()


def convertImgToGray(src_img, g_width = None, g_height = None):
    '''
    : 功能：将原始图片进行缩放转成灰度图，返回此灰度图
           adaBoostApp 使用
    '''
    if g_width and g_height:
        r_image = cv2.resize(src_img, (g_width, g_height))
    else:
        r_image = src_img
    
    r_gray_image = cv2.cvtColor(r_image, cv2.COLOR_RGB2GRAY)
    return r_gray_image


def os_path_check(dirs):
    '''
    : 功能：检查 dirs 路径是否存在，如果不存在就创建
    '''
    if not os.path.exists(dirs):
        os.makedirs(dirs)


def slice_image(src_img, x, y, w, h):
    return src_img[y:y+h, x:x+w] # 裁剪坐标为[y0:y1, x0:x1]


def p_adaBoostApp_biz(labelme_dict, positive_folder, train_set):
    '''
    : 功能：从 labelme_dict -> shapes 选出 label 与 train_set -> train_label 相同的 shape , 
           读取 labelme_dict -> imagePath 标识的图片，切取筛选出来的 shape 集，转灰度图后,
           存放二进制文件到 positive_folder -> grayscale 设定的文件夹下，将切取存放到该路径下的 /orig 下， 
           并将切图文件名写入 train_set -> train_folder 设定的文件夹下的 positive_path.txt；
    '''
    if not train_set['adaBoostApp-use']:
        return
    orig_path = positive_folder['original']
    gray_path = positive_folder['grayscale']
    full_path = os.path.abspath(os.path.join(orig_path, labelme_dict['imagePath']))
    orig_img = cv2.imread(full_path)

    slice_images = [] # 格式：[{'x': int, 'y': int, 'w': int, 'h': int}, ...]
    for el in labelme_dict['shapes']: # 遍历 labelme 标签
        if el['shape_type'] != 'rectangle':
            continue
        if el['label'] != train_set['train_label']:
            continue
        if len(el['points']) == 2: # 读取标注区域
            x1, y1 = el['points'][0]
            x2, y2 = el['points'][1]
            x = min(x1, x2)
            y = min(y1, y2)
            w = abs(x2 - x1)
            h = abs(y2 - y1)
            slice_images.append( {'x':int(x), 'y':int(y), 'w':int(w), 'h':int(h)} )
    
    serial = 0
    for el in slice_images:
        serial += 1
        slice_bin_path = os.path.abspath(os.path.join(gray_path, labelme_dict['imagePath'].split('.')[0]))
        slice_bin_path += '-binary-gray-' + str(serial)

        slice_img = slice_image(orig_img, el['x'], el['y'], el['w'], el['h'])
        gray_img = convertImgToGray(slice_img, train_set['app_pic_size']['width'], train_set['app_pic_size']['height'])
        saveBinaryImages(slice_bin_path, gray_img)
        
        txt_full_path = os.path.abspath(os.path.join(train_set['train_folder'], 'positive_path.txt'))
        f = open(txt_full_path, "a")
        f.write(slice_bin_path + '\n')
        f.close()


def n_adaBoostApp_biz(dir_file, negative_folder, train_set):
    '''
    : 功能：读取 full_path 文件，转灰度图后, 
           存放二进制文件到 negative_folder -> grayscale 设定的文件夹下， 
           并将切图文件名写入 train_set -> train_folder 设定的文件夹下的 negative_path.txt；
    '''
    if not train_set['adaBoostApp-use']:
        return
    
    orig_path = negative_folder['original']
    gray_path = negative_folder['grayscale']
    full_path = os.path.abspath(os.path.join(orig_path, dir_file))

    slice_bin_path = os.path.abspath(os.path.join(gray_path, dir_file.split('.')[0])) + '-binary-gray-n'
    orig_img = cv2.imread(full_path)
    gray_img = convertImgToGray(orig_img, train_set['app_pic_size']['width'], train_set['app_pic_size']['height'])
    saveBinaryImages(slice_bin_path, gray_img)

    txt_full_path = os.path.abspath(os.path.join(train_set['train_folder'], 'negative_path.txt'))
    f = open(txt_full_path, "a")
    f.write(slice_bin_path + '\n')
    f.close()


def p_opencv_biz(labelme_dict, positive_folder, train_set):
    '''
    : 功能：从 labelme_dict -> shapes 选出 label 与 train_set -> train_label 相同的 shape ,
           读取 labelme_dict -> imagePath 标识的图片，切取筛选出来的 shape集，
           将 shape集 写入 train_set -> train_folder 设定的文件夹下的 metre.info；
    '''
    if not train_set['openCV-use']:
        return

    orig_path = positive_folder['original']
    gray_path = positive_folder['grayscale']
    full_path = os.path.abspath(os.path.join(orig_path, labelme_dict['imagePath']))
    orig_img = cv2.imread(full_path)

    slice_images = [] # 格式：[{'x': int, 'y': int, 'w': int, 'h': int}, ...]
    for el in labelme_dict['shapes']: # 遍历 labelme 标签
        if el['shape_type'] != 'rectangle':
            continue
        if el['label'] != train_set['train_label']:
            continue
        if len(el['points']) == 2: # 读取标注区域
            x1, y1 = el['points'][0]
            x2, y2 = el['points'][1]
            x = min(x1, x2)
            y = min(y1, y2)
            w = abs(x2 - x1)
            h = abs(y2 - y1)
            slice_images.append( {'x':int(x), 'y':int(y), 'w':int(w), 'h':int(h)} )
    
    serial = 0
    info = full_path + ' ' + str(len(slice_images))
    for el in slice_images:
        serial += 1
        info += '  ' + str(el['x']) + ' ' + str(el['y']) + ' ' + str(el['w']) + ' ' + str(el['h'])
        if slice_test: # 调试用
            slice_img = slice_image(orig_img, el['x'], el['y'], el['w'], el['h'])
            name_spts =  labelme_dict['imagePath'].split('.')
            slice_path = os.path.abspath(os.path.join(gray_path, name_spts[0] + '-' + str(serial) + '.' + name_spts[1]))
            cv2.imwrite(slice_path, slice_img)
    
    metre_full_path = os.path.abspath(os.path.join(train_set['train_folder'], 'metre.info'))
    f = open(metre_full_path, "a")
    f.write(info + '\n')
    f.close()


def n_opencv_biz(full_path, negative_folder, train_set):
    '''
    : 功能：将 full_path 文件名写入 train_set -> train_folder 设定的文件夹下的 bg.txt；
    '''
    if not train_set['openCV-use']:
        return
    
    txt_full_path = os.path.abspath(os.path.join(train_set['train_folder'], 'bg.txt'))
    f = open(txt_full_path, "a")
    f.write(full_path + '\n')
    f.close()


def p_folder_traverse(positive_folder, train_set):
    '''
    : 功能：遍历 positive_folder -> original 文件夹 labelme 标注的json文件
    '''
    orig_path = positive_folder['original']
    gray_path = positive_folder['grayscale']

    if not os.path.exists(orig_path):
        return

    if gray_path:
        os_path_check(gray_path)

    if not os.path.exists(orig_path):
        return

    for dir_file in os.listdir(orig_path):
        full_path = os.path.abspath(os.path.join(orig_path, dir_file))
        if os.path.isdir(full_path): # 非文件
            pass
        elif dir_file.endswith('.json'): # labelme 标注文件
            with open(full_path, 'r') as label_file:
                label_dict = json.load(label_file)
            p_opencv_biz(label_dict, positive_folder, train_set) # openCV
            p_adaBoostApp_biz(label_dict, positive_folder, train_set) # adaBoostApp
        else:
            pass


def n_folder_traverse(negative_folder, train_set):
    '''
    : 功能：遍历 negative_folder -> original 文件夹 图片
    '''
    orig_path = negative_folder['original']
    gray_path = negative_folder['grayscale']

    if not os.path.exists(orig_path):
        return

    if gray_path:
        os_path_check(gray_path)

    for dir_file in os.listdir(orig_path):
        full_path = os.path.abspath(os.path.join(orig_path, dir_file))
        if os.path.isdir(full_path): # 非文件
            pass
        elif (dir_file.endswith('.png') or dir_file.endswith('.jpg') ): # 图片
            n_opencv_biz(full_path, negative_folder, train_set) # openCV
            n_adaBoostApp_biz(dir_file, negative_folder, train_set) # adaBoostApp
        else:
            pass


if __name__ == "__main__":
    with open("configure.json",'r') as load_f:
        cfg_dict = json.load(load_f)

    p_folder_traverse(cfg_dict['positive_folder'], cfg_dict['train_set'])
    n_folder_traverse(cfg_dict['negative_folder'], cfg_dict['train_set'])