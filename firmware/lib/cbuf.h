#ifndef CBUF_H
#define CBUF_H

// Inicializa o buffer circular
#define CBUF_Init( cbuf )     cbuf.m_putIdx = 0;

// Adiciona um elemento ao buffer
#define CBUF_Push( cbuf, elem ) \
    (cbuf.m_entry)[ cbuf.m_putIdx++ & ( cbuf##_SIZE - 1 ) ] = (elem)

// Lê um elemento do buffer
#define CBUF_Get( cbuf, index ) \
    (cbuf.m_entry)[ (cbuf.m_putIdx + (index)) & ( cbuf##_SIZE - 1 ) ]

#endif // CBUF_H
