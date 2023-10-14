#include "../include/SeamCarver.h"

#include <algorithm>
#include <cmath>
#include <limits>

SeamCarver::SeamCarver(Image image)
    : m_image(std::move(image))
{
}

const Image & SeamCarver::GetImage() const
{
    return m_image;
}

size_t SeamCarver::GetImageWidth() const
{
    return m_image.m_table.size();
}

size_t SeamCarver::GetImageHeight() const
{
    return (!m_image.m_table.empty()) ? m_image.m_table[0].size() : 0;
}

double SeamCarver::GetPixelEnergy(size_t x, size_t y) const
{
    size_t width = GetImageWidth();
    size_t height = GetImageHeight();
    return std::sqrt(std::pow(m_image.m_table[(x + 1) % width][y].m_red - m_image.m_table[(x - 1 + width) % width][y].m_red, 2) +
                     std::pow(m_image.m_table[(x + 1) % width][y].m_green - m_image.m_table[(x - 1 + width) % width][y].m_green, 2) +
                     std::pow(m_image.m_table[(x + 1) % width][y].m_blue - m_image.m_table[(x - 1 + width) % width][y].m_blue, 2) +
                     std::pow(m_image.m_table[x][(y + 1) % height].m_red - m_image.m_table[x][(y - 1 + height) % height].m_red, 2) +
                     std::pow(m_image.m_table[x][(y + 1) % height].m_green - m_image.m_table[x][(y - 1 + height) % height].m_green, 2) +
                     std::pow(m_image.m_table[x][(y + 1) % height].m_blue - m_image.m_table[x][(y - 1 + height) % height].m_blue, 2));
}

SeamCarver::PointerForDelete SeamCarver::CellOfDp(const size_t x,
                                                  const size_t y,
                                                  const bool is_vertical,
                                                  const std::vector<std::vector<SeamCarver::PointerForDelete>> & dp,
                                                  const size_t width,
                                                  const size_t height) const
{
    const size_t NULL_SIZE = 0;
    size_t xl, xr, yl, yr, xm = x, ym = y;

    if (!is_vertical) {
        xl = xr = xm = x - 1;
        yl = y == NULL_SIZE ? NULL_SIZE : y - 1;
        yr = y == height - 1 ? height - 1 : y + 1;
    }
    else {
        yl = yr = ym = y - 1;
        xl = x == NULL_SIZE ? NULL_SIZE : x - 1;
        xr = x == width - 1 ? width - 1 : x + 1;
    }

    if (dp[xl][yl].energy <= dp[xm][ym].energy && dp[xl][yl].energy <= dp[xr][yr].energy) {
        return PointerForDelete(dp[xl][yl].energy + GetPixelEnergy(x, y), xl, yl);
    }
    else if (dp[xm][ym].energy <= dp[xl][yl].energy && dp[xm][is_vertical ? ym : y].energy <= dp[xr][yr].energy) {
        return PointerForDelete(dp[xm][is_vertical ? ym : y].energy + GetPixelEnergy(x, y), xm, is_vertical ? ym : y);
    }
    else {
        return PointerForDelete(dp[xr][yr].energy + GetPixelEnergy(x, y), xr, yr);
    }
}

SeamCarver::Seam SeamCarver::FindSeam(const bool is_vertical) const
{
    size_t width = GetImageWidth();
    size_t height = GetImageHeight();

    std::vector<std::vector<PointerForDelete>> dp(width, std::vector<PointerForDelete>(height));

    size_t length_h = is_vertical ? width : height;
    size_t length_v = is_vertical ? height : width;

    for (size_t i = 0; i < length_h; i++) {
        dp[is_vertical ? i : 0][is_vertical ? 0 : i].energy = GetPixelEnergy(is_vertical ? i : 0, is_vertical ? 0 : i);
    }

    for (size_t y = 1; y < length_v; y++) {
        std::vector<SeamCarver::PointerForDelete> seam_e_row;

        for (size_t x = 0; x < length_h; x++) {
            seam_e_row.emplace_back(CellOfDp(is_vertical ? x : y, is_vertical ? y : x, is_vertical, dp, width, height));
        }
        for (size_t x = 0; x < length_h; x++) {
            dp[is_vertical ? x : y][is_vertical ? y : x] = seam_e_row[x];
        }
    }

    double mn = std::numeric_limits<double>::max();
    size_t mnxy = width - 1;

    for (size_t i = 0; i < (is_vertical ? width : height); i++) {
        if (dp[(is_vertical ? i : (width - 1))][(is_vertical ? (height - 1) : i)].energy < mn) {
            mn = dp[(is_vertical ? i : (width - 1))][(is_vertical ? (height - 1) : i)].energy;
            mnxy = i;
        }
    }
    Seam answer;
    answer.reserve(is_vertical ? height : width);
    if (!(width > 0 && height > 0)) {
        return answer;
    }
    answer.push_back(mnxy);

    for (size_t i = (is_vertical ? height : width) - 1; i > 0; i--) {
        is_vertical ? (mnxy = dp[mnxy][i].x) : (mnxy = dp[i][mnxy].y);
        answer.emplace_back(mnxy);
    }
    std::reverse(answer.begin(), answer.end());

    return answer;
}

SeamCarver::Seam SeamCarver::FindHorizontalSeam() const
{
    return FindSeam(false);
}

SeamCarver::Seam SeamCarver::FindVerticalSeam() const
{
    return FindSeam(true);
}

void SeamCarver::RemoveHorizontalSeam(const Seam & seam)
{
    size_t width = GetImageWidth();
    size_t height = GetImageHeight();

    if (width <= 0 || height <= 0) {
        return;
    }

    for (size_t i = 0; i < width; i++) {
        m_image.m_table[i].erase(m_image.m_table[i].begin() + seam[i]);
    }

    if (GetImageHeight() == 0) {
        m_image.m_table.pop_back();
    }
}

void SeamCarver::RemoveVerticalSeam(const Seam & seam)
{
    size_t width = GetImageWidth();
    size_t height = GetImageHeight();

    if (width <= 0 || height <= 0) {
        return;
    }

    for (size_t i = 0; i < height; i++) {
        for (size_t j = seam[i]; j < width - 1; j++) {
            m_image.m_table[j][i] = m_image.m_table[j + 1][i];
        }
    }

    m_image.m_table.pop_back();
}
