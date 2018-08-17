#pragma once

#include <vector>
#include <string>

namespace ceph
{
    class Image
    {
    private:

        std::vector<unsigned char> data_;
        int channels_;
        int wd_;
        int hgt_;

        void set(int wd, int hgt, int channels, const unsigned char* data);

    public:
        Image(int wd, int hgt, int channels, const unsigned char* data);
        Image(const std::string& file);
        const unsigned char* getData() const;
        int getWidth() const;
        int getHeight() const;
        int getChannels() const;
    };
};