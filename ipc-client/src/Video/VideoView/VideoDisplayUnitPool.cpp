#include "VideoView/VideoDisplayUnitPool.h"

#include <QtCore>

VideoDisplayUnitPool::VideoDisplayUnitPool(int poolSize)
    : m_poolSize(poolSize)
{
    for (int i = m_poolSize - 1; i >= 0; --i) {
        VideoDisplayUnit *video_display_unit = new VideoDisplayUnit(i);
        m_pool.push_back(video_display_unit);
    }
}

VideoDisplayUnitPool::~VideoDisplayUnitPool()
{
    for (auto widget : m_pool) {
        delete widget;
    }
}

VideoDisplayUnit* VideoDisplayUnitPool::acquire()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if (!m_pool.empty()) {
        VideoDisplayUnit* widget = m_pool.back();
        m_pool.pop_back();
        return widget;
    }

    return nullptr;  // 如果池中没有控件，返回nullptr
}

// 获取控件时指定父窗口
VideoDisplayUnit* VideoDisplayUnitPool::acquire(QWidget* parent)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if (!m_pool.empty()) {
        VideoDisplayUnit* widget = m_pool.back();
        widget->setParent(parent);  // 设置父控件
        m_pool.pop_back();
        return widget;
    }

    return nullptr;  // 如果池中没有控件，返回nullptr
}

void VideoDisplayUnitPool::release(VideoDisplayUnit* widget)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if (widget && m_pool.size() < static_cast<size_t>(m_poolSize)) {
        widget->setParent(nullptr);  // 释放控件的父控件
        m_pool.push_back(widget);
    }
}
