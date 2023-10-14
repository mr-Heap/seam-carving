#pragma once

#include "Image.h"

#include <cstdint>

class SeamCarver
{
    using Seam = std::vector<size_t>;

public:
    SeamCarver(Image image);

    /**
     * Returns current image
     */
    const Image & GetImage() const;

    /**
     * Gets current image width
     */
    size_t GetImageWidth() const;

    /**
     * Gets current image height
     */
    size_t GetImageHeight() const;

    /**
     * Returns pixel energy
     * @param columnId column index (x)
     * @param rowId row index (y)
     */
    double GetPixelEnergy(size_t columnId, size_t rowId) const;

    /**
     * Returns sequence of pixel row indexes (y)
     * (x indexes are [0:W-1])
     */
    Seam FindHorizontalSeam() const;

    /**
     * Returns sequence of pixel column indexes (x)
     * (y indexes are [0:H-1])
     */
    Seam FindVerticalSeam() const;

    /**
     * Removes sequence of pixels from the image
     */
    void RemoveHorizontalSeam(const Seam & seam);

    /**
     * Removes sequence of pixes from the image
     */
    void RemoveVerticalSeam(const Seam & seam);

    struct PointerForDelete
    {
        double energy = 0.0;
        size_t x = SIZE_MAX;
        size_t y = SIZE_MAX;
        PointerForDelete(double energy = 0.0, size_t x = SIZE_MAX, size_t y = SIZE_MAX)
            : energy(energy)
            , x(x)
            , y(y)
        {
        }
    };

private:
    Image m_image;

    Seam FindSeam(bool is_vertical) const;

    SeamCarver::PointerForDelete CellOfDp(size_t x,
                                          size_t y,
                                          bool flag,
                                          const std::vector<std::vector<SeamCarver::PointerForDelete>> & dp,
                                          size_t width,
                                          size_t height) const;
};