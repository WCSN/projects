/*
 * Title       : DynamicBuffer.h
 * Date        : 26.07.2010
 * Author      : Andrei Borodaenko
 * Description :
 */

#ifndef DYNAMICBUFFER_H
#define DYNAMICBUFFER_H

#include <cstring>
#include <string>

namespace mt {

template<typename T>
class DynamicBuffer {

public:
    /** \name Constructors & Destructor */
    //@{
    /**
     * Default constructor.
     */
    DynamicBuffer() : m_capacity(0), m_size(0), m_data(0) { }

    /**
     * Creates buffer with given capacity.
     * \param capacity Dynamic Buffer initial capacity.
     */
    DynamicBuffer(size_t capacity) : m_capacity(capacity), m_size(0), m_data(new T[capacity]) {
        init();
    }

    /**
     * Copy constructor.
     */
    DynamicBuffer(const DynamicBuffer<T>& source) : m_capacity(source.m_capacity),
                                                    m_size(source.m_size),
                                                    m_data(new T[source.m_capacity]) {
        copy(source);
    }

    /**
     * Destructor.
     */
    ~DynamicBuffer() { delete[] m_data; }
    //@}

    /** \name Operators */
    //@{
    operator T*() { return m_data; }

    operator const T*() const { return m_data; }

    /**
     * Assignment operator.
     */
    DynamicBuffer<T>& operator=(const DynamicBuffer<T>& source) {
        if (this != &source) {
            copy(source);
        }

        return *this;
    }
    //@}



    /** \name Functions */
    //@{
    /**
     * Appends \a count elements to currently stored data.
     * \param data - pointer to start of the data to append.
     * \param count - number of elements to append.
     */
    void append(const T* data, size_t count) {
        if (m_size + count > m_capacity) {
            reallocateSave(m_size + count);
        }
        std::copy(data, data + count, m_data + m_size);
        m_size += count;
    }

    void append(const DynamicBuffer<T> buffer) {
        append(buffer.m_data, buffer.m_size);
    }

    /**
     * Sets initial all-zero values. Releases allocated internal memory.
     */
    void clear() {
        delete[] m_data;
        m_data = 0;
        m_capacity = 0;
        m_size = 0;
    }

    /**
     * Delete all data. Reallocate memory. Size sets to 0.
     */
    void reallocate(size_t newCapacity) {
        if (newCapacity > 0) {
            clear();
            m_data = new T[newCapacity];
            m_capacity = newCapacity;
            m_size = 0;
        }
    }

    /**
     * Reallocates internal buffer. Currently stored data are saved.
     * Reallocates if and only if \a newCapacity is greater then current size.
     */
    void reallocateSave(size_t newCapacity) {
        if (newCapacity > m_size) {
            T* newData = new T[newCapacity];
            std::copy(m_data, m_data + m_size, newData);
            delete[] m_data;
            m_data = newData;
            m_capacity = newCapacity;
        }
    }


    T* getDataPtr() { return m_data; }

    const T* getDataPtr() const { return m_data; }

    size_t getSize() const { return m_size; }

    size_t getCapacity() const { return m_capacity; }

    /**
     * Sets data size, e.g. number of actual stored elements.
     */
    void setSize(size_t size) { m_size = size; }

    /** \name Friends */
    //@{
    template<typename U>
    friend bool operator==(const DynamicBuffer<U>& lhs, const DynamicBuffer<U>& rhs);
    //@}

protected:
    /** \name Functions */
    void init() {
        for (size_t i = 0; i < m_capacity; ++i) { m_data[i] = T(); }
    }


    void copy(const DynamicBuffer<T>& source) {
        if (m_capacity < source.m_size) { reallocate(source.m_size); }
        std::copy(source.m_data, source.m_data + source.m_size, m_data);
        setSize(source.m_size);
    }

private:
    size_t m_capacity;
    size_t m_size;
    T* m_data;
};

template<typename U>
bool operator==(const DynamicBuffer<U>& lhs, const DynamicBuffer<U>& rhs) {
    if (lhs.m_size != rhs.m_size) { return false; }
    for (size_t i = 0; i < lhs.m_size; ++i) {
        if (lhs.m_data[i] != rhs.m_data[i]) { return false; }
    }
    return true;
}

typedef mt::DynamicBuffer<uint8_t> ByteBuffer;

} // namespace mt

#endif /* DYNAMICBUFFER_H */
