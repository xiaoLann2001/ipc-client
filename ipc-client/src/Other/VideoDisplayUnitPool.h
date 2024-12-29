#ifndef VIDEODISPLAYUNITPOOL_H
#define VIDEODISPLAYUNITPOOL_H

#include "VideoView/VideoDisplayUnit.h"
#include <list>
#include <mutex>

class VideoDisplayUnitPool
{
public:
    explicit VideoDisplayUnitPool(int poolSize);
    ~VideoDisplayUnitPool();

    VideoDisplayUnit* acquire();
    VideoDisplayUnit* acquire(QWidget* parent);
    void release(VideoDisplayUnit* widget);

private:
    std::list<VideoDisplayUnit*> m_pool;
    std::mutex m_mutex;
    int m_poolSize;
};

#endif // VIDEODISPLAYUNITPOOL_H
