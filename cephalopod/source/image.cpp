#define STB_IMAGE_IMPLEMENTATION
#include "../include/cephalopod/image.hpp"
#include "stb_image.h"

ceph::Image::Image(int wd, int hgt, int channels, std::vector<unsigned char>&& data) :
	wd_(wd), hgt_(hgt), channels_(channels), data_(std::move(data))
{
}

ceph::Image::Image(int wd, int hgt, int channels, const unsigned char* data)
{
    set(wd, hgt, channels, data);
}

ceph::Image::Image(const std::string& file)
{
    int wd; 
    int hgt; 
    int channels; 
    unsigned char* data = stbi_load(file.c_str(), &wd, &hgt, &channels, 0);
    if (!data) 
        throw std::runtime_error(stbi_failure_reason());
    set(wd, hgt, channels, data);
    stbi_image_free(data);
}

void ceph::Image::set(int wd, int hgt, int channels, const unsigned char* data)
{
    wd_ = wd; 
    hgt_ = hgt;
    channels_ = channels;

    size_t sz = wd * hgt * channels;
    data_.resize(sz);
    std::copy(data, data + sz, &data_[0]);
}

const unsigned char* ceph::Image::getData() const
{
    return &(data_[0]);
}

int ceph::Image::getWidth() const
{
    return wd_;
}

int ceph::Image::getHeight() const
{
    return hgt_;
}

int ceph::Image::getChannels() const
{
    return channels_;
}
