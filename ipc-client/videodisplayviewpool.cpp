#include "videodisplayviewpool.h"

#include <QtCore>

VideoDisplayViewPool::VideoDisplayViewPool(int poolSize)
    : m_poolSize(poolSize)
{
    for (int i = 0; i < m_poolSize; ++i) {
        VideoDisplayUnit *video_display_unit = new VideoDisplayUnit(i);
        m_pool.push_back(video_display_unit);
    }
}

VideoDisplayViewPool::~VideoDisplayViewPool()
{
    for (auto widget : m_pool) {
        delete widget;
    }
}

VideoDisplayUnit* VideoDisplayViewPool::acquire()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if (!m_pool.empty()) {
        VideoDisplayUnit* widget = m_pool.back();
        m_pool.pop_back();
        return widget;
    }

    return nullptr;  // 如果池中没有控件，返回nullptr
}

void VideoDisplayViewPool::release(VideoDisplayUnit* widget)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if (widget && m_pool.size() < m_poolSize) {
        m_pool.push_back(widget);
    }
}
