#ifndef __COMMON__MEMORY_BUFFER__H
#define __COMMON__MEMORY_BUFFER__H

#include <malloc.h>
#include <string.h>
#include <stdint.h>

#include <mt/common/Exception.h>

namespace mt {
	/** 
	 *  @class MemoryBuffer
	 *	@brief Dynamic buffer
	 */

	template <class T>
		class MemoryBuffer {

	public:
		//@{ @name Constructors and destructor
		/**
		 * @param count Size of buffer.
		 */
		MemoryBuffer(unsigned int count = 0);

		/**
		 * Copy constructor
		 * @param src Copied MemoryBuffer
		 */
		MemoryBuffer<T>(const MemoryBuffer<T>& src);
		virtual ~MemoryBuffer();
		//@}

		//@{ @name Operators
		operator const T* () const;
		operator T* ();
		/**
		 * Assignment operator.
		 * @throws mt::Exception if an error occurred at allocating memory.
		 */
		MemoryBuffer<T>& operator=(const MemoryBuffer<T>& src);
		//@}

		/**
		 * Current size of buffer.
		 */
		uint32_t length() const;

		/**
		 * Append the buffer.
		 *
		 * @param src Buffer for append.
		 * @param size Count of src's elements to append
		 *
		 * @throws mt::Exception if an error occurred at memory reallocating.
		 * @return Size of new buffer
		 */
		uint32_t append(T* src, uint32_t size);

		/**
		 * Append the buffer.
		 *
		 * @param src MemoryBuffer for append.
		 *
		 * @throws mt::Exception if an error occurred at memory reallocating.
		 * @return Size of new buffer
		 */
		uint32_t append(const MemoryBuffer<T>& src);

		/**
		 * Truncate the buffer.
		 *
		 * @param startPos Start position of truncate.
		 * @param length Length of truncated buffer. By default set to '-1'. In that case the buffer truncated from startPos to end of buffer.
		 *
		 * @throws mt::Exception if startPos is less then 0 or more that size of the buffer.
		 * @throws mt::Exception if length not set to default and startPos+length more than size of the buffer.
		 * @throws mt::Exception if an error occurred at allocating memory.
		 * @return Size of new buffer
		 */
		uint32_t truncate(uint32_t startPos, int32_t length = -1);

	private:
		T* m_buffer;								/**< Pointer to date. */
		uint32_t m_length;					/**< Size of the buffer. */
	};

	template <class T>
		MemoryBuffer<T>::MemoryBuffer(uint32_t count) : m_buffer(0), m_length(count){
			if(m_length){
				m_buffer = (T*)malloc(count * sizeof(T));
				if(!m_buffer)
					MT_EXCEPTION(mt::Exception, "MemoryBuffer: Failed to allocate " << count << " units.");
			}
	}

	template <class T>
		MemoryBuffer<T>::MemoryBuffer(const MemoryBuffer<T>& src) : m_buffer(0), m_length(0){
		operator=(src);
	}

	template <class T>
		MemoryBuffer<T>::~MemoryBuffer<T>(){
		if(m_buffer)
			free((void*)m_buffer);
		}

	template <class T>
	MemoryBuffer<T>::operator const T* () const {
		return m_buffer;
	}

	template <class T>
		MemoryBuffer<T>::operator T* (){
		return m_buffer;
	}

	template <class T>
		uint32_t MemoryBuffer<T>::length() const {
		return m_length;
	}

	template <class T>
		uint32_t MemoryBuffer<T>::append(T* src, uint32_t size ){
		if(0==(m_buffer=(T*)realloc((void*)m_buffer,sizeof(T)*(m_length+size)))) {
			MT_EXCEPTION(mt::Exception, "MemoryBuffer<>::append Failed to allocate " << m_length + size<< " of type '" <<  typeid(T).name() << "'");
		} else {
			memcpy(m_buffer+m_length, src, size*sizeof(T));
			m_length += size;
		}
		return m_length;
	}

	template <class T>
		uint32_t MemoryBuffer<T>::append(const MemoryBuffer<T>& src ){
		return append( src, src.m_length);
	}

	template <class T>
		MemoryBuffer<T>& MemoryBuffer<T>::operator=(const MemoryBuffer<T>& src){
		if(m_buffer)
			free((void*)m_buffer);

		m_length=src.m_length;
		m_buffer=(T*)malloc(m_length*sizeof(T));
		if(!m_buffer) {
			MT_EXCEPTION(mt::Exception, "MemoryBuffer<>::failed to acclocate memory. Requested size=" << m_length << " elements of '"
									 << typeid(T).name() << "'");
		}
		memcpy(m_buffer,src.m_buffer,m_length*sizeof(T));
		return *this;
	}

	template <class T>
		uint32_t MemoryBuffer<T>::truncate(uint32_t startPos, int32_t length) {
			if( startPos<0 || startPos>m_length )
				MT_EXCEPTION(mt::Exception, "MemoryBuffer: Out of bounds.current length=" << m_length << " but start=" << startPos);

			if( length!=-1 && startPos+length>m_length ) {
				MT_EXCEPTION(mt::Exception, "MemoryBuffer: Out of bounds.Start " << startPos << " + Length="
										 << length << " exceeds current length of data " << m_length );
			}

			if( -1==length )
				length = m_length - startPos;
			T* newbuf=(T*)malloc(length*sizeof(T));
			if(0==newbuf)
				MT_EXCEPTION(mt::Exception, "MemoryBuffer: Failed to allocate " << length << " units of '" << typeid(T).name() << "'");
			memcpy( newbuf, m_buffer+startPos, length );
			free((void*)m_buffer);
			m_buffer = newbuf;
			m_length = length;
			return m_length;
		}
}
#endif /* __COMMON__MEMORY_BUFFER__H */
