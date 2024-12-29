#ifndef CUSTOMQWIDGETPOOL_H
#define CUSTOMQWIDGETPOOL_H

#include <QtCore>
#include <QMutex>
#include <QMutexLocker>
#include <QList>
#include <QWidget>

// 模板类的方法定义需要在头文件，否则会链接错误；或者在使用的地方显式实例化模板类

/**
 * @brief 自定义窗口池，注意：此类是一个模板类，不能是 QObject 的子类，窗口池中的窗口必须具有默认构造函数
 * @tparam T 窗口类型
 */
template <typename T>
class CustomQWidgetPool {
public:
    // 构造函数，创建指定大小的窗口池
    explicit CustomQWidgetPool(int poolSize, QWidget *parent = nullptr);

    // 析构函数，销毁所有池中的窗口
    ~CustomQWidgetPool();

    // 获取一个窗口，如果池中没有窗口，则返回 nullptr
    T* acquire();

    // 获取窗口并设置父窗口
    T* acquire(QWidget *parent);

    // 释放一个窗口
    void release(T* obj);

    // 释放池中的所有窗口
    void clear();

    // 调整池大小
    void resize(int newSize);

private:
    int m_poolSize;     // 池的大小
    QList<T*> m_pool;   // 存储池中窗口的容器
    QMutex m_mutex;     // 用于线程同步
};

template <typename T>
CustomQWidgetPool<T>::CustomQWidgetPool(int poolSize, QWidget *parent)
    : m_poolSize(poolSize) {
    for (int i = 0; i < m_poolSize; ++i) {
        T *obj = new T();  // 创建 T 类型的窗口
        obj->setParent(parent);  // 设置父窗口
        m_pool.append(obj);
    }
}

template <typename T>
CustomQWidgetPool<T>::~CustomQWidgetPool() {
    clear();
}

template <typename T>
T* CustomQWidgetPool<T>::acquire() {
    QMutexLocker locker(&m_mutex);

    if (!m_pool.isEmpty()) {
        T* obj = m_pool.takeLast();
        return obj;
    }

    return nullptr;  // 如果池中没有窗口，返回 nullptr
}

template <typename T>
T* CustomQWidgetPool<T>::acquire(QWidget *parent) {
    QMutexLocker locker(&m_mutex);

    if (!m_pool.isEmpty()) {
        T* obj = m_pool.takeLast();
        obj->setParent(parent);  // 设置父窗口
        return obj;
    }

    return nullptr;  // 如果池中没有窗口，返回 nullptr
}

template <typename T>
void CustomQWidgetPool<T>::release(T* obj) {
    QMutexLocker locker(&m_mutex);

    if (obj && m_pool.size() < m_poolSize) {
        obj->setParent(nullptr);  // 释放父窗口的关系
        m_pool.append(obj);
    }
}

template <typename T>
void CustomQWidgetPool<T>::clear() {
    QMutexLocker locker(&m_mutex);

    for (T* obj : qAsConst(m_pool)) {
        obj->deleteLater();  // 删除窗口
    }
    m_pool.clear();
}

template <typename T>
void CustomQWidgetPool<T>::resize(int newSize) {
    QMutexLocker locker(&m_mutex);

    if (newSize > m_poolSize) {
        // 增加池的大小
        for (int i = m_poolSize; i < newSize; ++i) {
            T* obj = new T();  // 创建 T 类型的窗口
            m_pool.append(obj);
        }
    } else if (newSize < m_poolSize) {
        // 减少池的大小
        while (m_pool.size() > newSize) {
            T* obj = m_pool.takeLast();
            obj->deleteLater();  // 删除窗口
        }
    }

    m_poolSize = newSize;
}

#endif // CUSTOMQWIDGETPOOL_H
