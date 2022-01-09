//
// Nome file:	xppCircularQueue.h
//
// Descrizione:	Dichiarazione oggetto struttura dati FIFO a buffer circolare
//
// Storia:
// Data		Autore	Commento
// ---------------------------------------------------------------------------
// 03-07-12	SR		Prima stesura
//
#ifndef __XPP_CIRCULAR_QUEUE_H__
#define __XPP_CIRCULAR_QUEUE_H__
#pragma once


//
// Oggetto struttura dati FIFO a buffer circolare
//
template <class T> class CxppCircolarQueue
{
public:
	//
	// Costruttore(i) / distruttore
	//
	CxppCircolarQueue()												{ __Init( 0 ); };
	CxppCircolarQueue( unsigned int uiSize )						{ assert( uiSize > 0 ); __Init( uiSize ); };
	~CxppCircolarQueue()
	{
		if ( m_pBuffer )
			delete [] m_pBuffer;
	};

	//
	// Funzioni
	//
	T&				back( void );
	const T&		back( void ) const;
	T&				back( unsigned int iElemIdx );
	const T&		back( unsigned int iElemIdx ) const;
	void			clear( void );
	bool			empty( void ) const								{ return( ( m_uiElementsCounter == 0 ) ? true : false ); };
	T&				front( void );
	const T&		front( void ) const;
	T&				front( unsigned int iElemIdx );
	const T&		front( unsigned int iElemIdx ) const;
	unsigned int	max_size( void ) const							{ return( m_uiBufferSize ); };
	unsigned int	overrun_counter( void ) const					{ return( m_uiOverrunCounter ); };
	void			pop( void );
	bool			push( const T& _Val );
	void			reserve( unsigned int uiSize );
	int				set_overrun_mode( int iOverrunMode )			{ m_uiOverrunMode = iOverrunMode; return( false ); };
	unsigned int	size( void ) const								{ return( m_uiElementsCounter ); };
protected:
	void			__Init( unsigned int uiSize );

	//
	// Variabili
	//
protected:
	T*				m_pBuffer;										// Buffer circolare
	unsigned int	m_uiBufferSize;									// Dimensione buffer
	unsigned int	m_uiElementsCounter;							// Numero elementi presenti in coda
	unsigned int	m_uiReadIdx;									// Indice per lettura elemento
	unsigned int	m_uiWriteIdx;									// Indice per scrittura elemento
	unsigned int	m_uiOverrunMode;								// Modalita' gestione overrun
	unsigned int	m_uiOverrunCounter;								// Contatore scritture perse per overrun
	T				m_ErrElem;										// Elemento vuoto per gestione errori
};


template< class T > void CxppCircolarQueue< T >::__Init( unsigned int uiSize )
{
	if ( uiSize == 0 )
	{
		m_pBuffer = NULL;
		m_uiBufferSize = 0;
	}
	else
	{
		m_pBuffer = new T[ uiSize ];
		m_uiBufferSize = uiSize;
	}

	m_uiElementsCounter = 0;										// Numero elementi presenti in coda
	m_uiReadIdx = 0;												// Indice per lettura elemento
	m_uiWriteIdx = 0;												// Indice per scrittura elemento
	m_uiOverrunMode = 0;											// Modalita' gestione overrun
	m_uiOverrunCounter = 0;											// Contatore scritture perse per overrun
};


template< class T > T& CxppCircolarQueue< T >::back( void )
{ 
	assert( m_pBuffer && m_uiBufferSize > 0 );

	if ( m_uiElementsCounter > 0 )
	{
		if ( m_uiWriteIdx > 0 )
			return( m_pBuffer[m_uiWriteIdx-1] );
		else
			return( m_pBuffer[m_uiBufferSize-1] );
	}
	else
	{
		assert( false );
		return( m_ErrElem );
	}
}


template< class T > const T& CxppCircolarQueue< T >::back( void ) const
{ 
	assert( m_pBuffer && m_uiBufferSize > 0 );

	if ( m_uiElementsCounter > 0 )
	{
		if ( m_uiWriteIdx > 0 )
			return( m_pBuffer[m_uiWriteIdx-1] );
		else
			return( m_pBuffer[m_uiBufferSize-1] );
	}
	else
	{
		assert( false );
		return( m_ErrElem );
	}
}


template< class T > T& CxppCircolarQueue< T >::back( unsigned int iElemIdx )
{ 
	assert( m_pBuffer && m_uiBufferSize > 0 );

	if ( m_uiElementsCounter > 0 && iElemIdx < m_uiElementsCounter )
	{
		iElemIdx = ( m_uiBufferSize + m_uiWriteIdx - 1 - iElemIdx ) % m_uiBufferSize;
		return( m_pBuffer[iElemIdx] );
	}
	else
	{
		assert( false );
		return( m_ErrElem );
	}
}


template< class T > const T& CxppCircolarQueue< T >::back( unsigned int iElemIdx ) const
{ 
	assert( m_pBuffer && m_uiBufferSize > 0 );

	if ( m_uiElementsCounter > 0 && iElemIdx < m_uiElementsCounter )
	{
		iElemIdx = ( m_uiBufferSize + m_uiWriteIdx - 1 - iElemIdx ) % m_uiBufferSize;
		return( m_pBuffer[iElemIdx] );
	}
	else
	{
		assert( false );
		return( m_ErrElem );
	}
}


template< class T > void CxppCircolarQueue< T >::clear( void )
{ 
	m_uiElementsCounter = 0;										// Numero elementi presenti in coda
	m_uiReadIdx = 0;												// Indice per lettura elemento
	m_uiWriteIdx = 0;												// Indice per scrittura elemento
	m_uiOverrunCounter = 0;											// Contatore scritture perse per overrun
}


template< class T > T& CxppCircolarQueue< T >::front( void )
{ 
	assert( m_pBuffer && m_uiBufferSize > 0 );

	if ( m_uiElementsCounter > 0 )
		return( m_pBuffer[m_uiReadIdx] );
	else
	{
		assert( false );
		return( m_ErrElem );
	}
}


template< class T > const T& CxppCircolarQueue< T >::front( void ) const
{ 
	assert( m_pBuffer && m_uiBufferSize > 0 );

	if ( m_uiElementsCounter > 0 )
		return( m_pBuffer[m_uiReadIdx] );
	else
	{
		assert( false );
		return( m_ErrElem );
	}
}


template< class T > T& CxppCircolarQueue< T >::front( unsigned int iElemIdx )
{ 
	assert( m_pBuffer && m_uiBufferSize > 0 );

	if ( m_uiElementsCounter > 0 && iElemIdx < m_uiElementsCounter )
	{
		iElemIdx = ( m_uiReadIdx + iElemIdx ) % m_uiBufferSize;
		return( m_pBuffer[iElemIdx] );
	}
	else
	{
		assert( false );
		return( m_ErrElem );
	}
}


template< class T > const T& CxppCircolarQueue< T >::front( unsigned int iElemIdx ) const
{ 
	assert( m_pBuffer && m_uiBufferSize > 0 );

	if ( m_uiElementsCounter > 0 && iElemIdx < m_uiElementsCounter )
	{
		iElemIdx = ( m_uiReadIdx + iElemIdx ) % m_uiBufferSize;
		return( m_pBuffer[iElemIdx] );
	}
	else
	{
		assert( false );
		return( m_ErrElem );
	}
}


template< class T > void CxppCircolarQueue< T >::pop( void )
{
	// Controlla che vi sia almeno un elemento in coda
	assert( m_pBuffer && m_uiBufferSize > 0 );
	if ( m_uiElementsCounter > 0 )
	{
		// Aggiorna numero elementi in coda e indice lettura dati
		--m_uiElementsCounter;
		m_uiReadIdx = ++m_uiReadIdx % m_uiBufferSize;

		// Azzera il contatore dati persi per overrun
		m_uiOverrunCounter = 0;
	}
}


template< class T > bool CxppCircolarQueue< T >::push( const T& _Val )
{
	// Controlla che vi sia spazio disponibile nella coda dati eventi
	assert( m_pBuffer && m_uiBufferSize > 0 );
	if ( m_uiElementsCounter >= m_uiBufferSize )
	{
		if ( m_uiOverrunMode == 0 )
		{
			++m_uiOverrunCounter;									// Contatore scritture perse per overrun
			return( true );
		}
		else if ( m_uiOverrunMode == 1 )
			pop();													// Se m_uiOverrunMode = 1, rimuovi l'elemento piu' vecchio
		else
			assert( false );
	}

	// Inserimento dato in coda e aggiornamento indice scrittura dati
	m_pBuffer[m_uiWriteIdx] = _Val;
	++m_uiElementsCounter;
	m_uiWriteIdx = ++m_uiWriteIdx % m_uiBufferSize;

	return( false );
}


template< class T > void CxppCircolarQueue< T >::reserve( unsigned int uiSize )
{
	assert( uiSize > 0 );

	if ( m_uiBufferSize != 0 && m_pBuffer )
	{
		delete [] m_pBuffer;
		m_pBuffer = NULL;
		m_uiBufferSize = 0;
	}

	__Init( uiSize );
};


#endif	// __CIRCULAR_QUEUE_H__
