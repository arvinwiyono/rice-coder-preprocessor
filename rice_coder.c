#include <stdio.h>
/*******
 *******
 ****
 ****   Sample code routines to handle bitwise i/o in C.
 ****
 *******
 *******/


/****
 ****
 **  The typedefs indicate assumptions made about the size of unsigned
 **  integers and the number of bits they hold.
 ****/
      typedef  unsigned char	     INT8     ;
      typedef  unsigned long int     INT32     ;

/****
 ****
 **  This structure holds leftover bits.  To initialise, bit_number should
 **  be zero.  Otherwise bit_number gives the number of valid bits in
 **  'string'.  Bits are stored right justified with a zero left fill.
 ****/

      typedef struct BIT_BUFFER_T {
            INT8      bit_number;
            INT32     string;
            } BIT_BUFFER;


int	Get_Bits (   FILE        * Input,   int     Bits_Wanted,
                     BIT_BUFFER  * buffer,  INT32 * Bit_String
                 )
{
  /****
   **
   **  This routine is asked to get 'Bits_Wanted' bits from the stream
   **  'Input'.   The number of valid bits is returned via the function
   **  name.  Bits are first retrieved from 'buffer' before retrieving 
   **  bits from 'Input'.   It is assumed that the number of bits which
   **  are requested is no greater than 32.
   **
   ****/
   register   Have, Want, Move, Left; 
   int        Char;

   Have = 0;
   Want = Bits_Wanted;
   if ( ( Want < 1 ) || ( Want > 32 ) )
      return 0;

   if ( Want < buffer->bit_number )
       Move = Want;
   else
       Move = buffer->bit_number;

        /***  move the bits from the buffer   ****/
   Left               = buffer->bit_number - Move;
   * Bit_String       = buffer->string >> Left;
   buffer->bit_number  = Left;
   buffer->string      = buffer->string & ( (1 << Left ) - 1 );
   Have		      = Move;
     
       /****
	****
	** Now, if we need bits they will have to come from 'Input'.
	** Note: this routine reads a byte at a time but reading
	** more bytes at a time may be more efficient.
	****
	****/
        
   /***  this code assumes the buffer has fewer than 8 bits in it
    ***  and so any extra bits have to come from the 'Input'.
    ***/
   while ( Have < Bits_Wanted )
     {
       Char = getc ( Input );
       if ( Char == EOF ) break;

       buffer->string = Char;
       Want          = Bits_Wanted - Have;
       if ( Want < 8 )
	 {  
	   Move = Want;
           Left = 8 - Move;
           * Bit_String = ( ( * Bit_String ) << Move )
                          | (buffer->string >> Left);
           buffer->bit_number  = Left;
           buffer->string      = buffer->string & ( (1 << Left ) - 1 );
           Have += Move;
	 }  
	else
	 {  
	   * Bit_String   = ( (* Bit_String ) << 8 ) | buffer->string;
	   Have          += 8;
           buffer->string = 0;
	 }  

      }
     /*****
      *****
      **
      ** If we get this far, we have either read as many bits into
      ** 'Bit_String' as we need or we have reached End of File.
      ** The calling program can detect End-of-File if the number
      ** of bits it requested is less than the number of bits it got.
      *****
      *****/
      return ( Have );
}

int	Put_Bits (   FILE        * Output,  int   Bits_Wanted,
                     BIT_BUFFER  * buffer,  INT32 Bit_String
                 )
{
  /*****
   *****
   **  This routine is given a bit pattern in 'Bit_String' and
   **  writes it onto the output stream given by 'Output'.
   **  The number of bits to be written is given by Bits_Wanted.
   **  It is assumed that bits are written one byte at a time
   **  and that leftover bits are stored in 'buffer'.
   **
   *****
   *****/
   register    To_Go, Move, Left, Mask;

   To_Go = Bits_Wanted;

   /***
    *** the following code assumes that the buffer has fewer
    *** than 8 bits in it.
    ***/
   while ( (buffer->bit_number) + To_Go > 7 )
     {
	Move          = 8 - buffer->bit_number;
	Left          = To_Go - Move;
	buffer->string = ( ( buffer->string) << Move ) | 
			 ( Bit_String >> Left );

	putc ( buffer->string, Output );

	buffer->bit_number = 0;
	buffer->string     = 0;
	Mask              = ( 1 << To_Go ) - ( 1 << Left );
	Bit_String        = Bit_String & (~Mask);
	To_Go            -= Move;
     }

    buffer->string      =  ( ( buffer->string) << To_Go ) | Bit_String ;
    (buffer->bit_number) += To_Go;
    return (0);

}




int	Flush_Bits (   FILE * Output, BIT_BUFFER  * buffer
                   )
{
   
  /*****
   *****
   **  This routine flushed any remaining bits in 'buffer'
   **  onto the output stream, 'Output'.
   **  This routine should still function correctly even if 'buffer'
   **  contains more than a byte of bits.
   **
   *****
   *****/
   register    To_Go, Move, Left, Mask;

   To_Go = buffer->bit_number;

   while ( To_Go > 7 )
     {
	Move          = 8;
	Left          = To_Go - 8;

	putc ( buffer->string >> Left, Output );

	Mask          = ( 1 << To_Go ) - ( 1 << Left );
	buffer->string = buffer->string & (~Mask);
	To_Go         = Left;
     }

    /*****
     *****
     **  Fill any unused bit positions on the end with 1 bits.
     **  (only do this if the last byte is incomplete).
     *****
     *****/
    if ( To_Go == 0 )
      return ( 0 );

    buffer->string = buffer->string << ( 8 - To_Go );
    buffer->string = buffer->string | ( ( 1 << ( 8 - To_Go ) ) - 1 );
    putc ( buffer->string, Output );
    buffer->string     = 0;
    buffer->bit_number = 0;
    return ( 0 );
}

    int   Select_K   ( int block [],
    		       int Number_of_Symbols,
    		       int Max_Bits )
{
 /*****
  *****
  **  Assume symbols are in the range 1 .. (2 ^ Max_Bits)-1.
  **  Symbols are in the buffer in the positions 0 .. (Number_of_Symbols-1).
  **  Max_Bits cannot exceed 16 without resizing 'sum'.
  **
  *****
  *****/
  register   i, j, reduced_symbol;
  int	     sum [ 17 ], best_sum, long_codes;

  for ( i=0; i < Max_Bits; i ++ )
     sum [ i ] = 0;

  for ( i=0; i < Number_of_Symbols; i++ )
     {
       reduced_symbol = block [ i ];

       for ( j=0; j < Max_Bits -1; j ++ )
          {
            sum [ j ] += reduced_symbol + j + 1;
            reduced_symbol >>= 1;
          }
     }
 /*****
  *****
  **  Find the smallest bit sum.
  **
  *****
  *****/
    i = 0;
    best_sum = sum [ 0 ];

    for ( j=1; j < Max_Bits-1; j++ )
       {
         if ( sum [ j ] < best_sum )
           {
              i        = j;
              best_sum = sum [ j ];
           }
       }
 /*****
  *****
  **  It is assumed that the symbols are in the range 0 .. (2^Max_Bits).
  **  Thus there is one more character than can be represented in Max_Bits
  **  worth of bits.  This extra character can serve as either an escape
  **  character or an end-of-file marker.
  **  The case where the coding parameter is equal to Max_Bits -1 is a special
  **  case.  In this case, each code will be output in Max_Bits if it has
  **  a value < (2^ Max_Bits)-1.  There are two remaining possibilities
  **  for the character and an extra bit is used to select between the two
  **  possibilities.
  **
  *****
  *****/
    if ( i == Max_Bits - 2 )
      {
	reduced_symbol = ( 1 << Max_Bits ) -1;
	long_codes = 0;
	for ( j = 0; j < Number_of_Symbols; j ++ )
	   if ( block [ j ] >= reduced_symbol )
	     long_codes ++;

	if ( ( Max_Bits * Number_of_Symbols + long_codes ) < best_sum )
	  i = Max_Bits -1;
      }
    return ( i );
 /*****
  *****
  **  We may want to treat 'k == Max_Bits -1' as a special case.
  **
  *****
  *****/
}

   Encode_Stream ( FILE * Input,
                   FILE * Output,
                   int    MaxBits )
{
 /*****
  *****
  **  This routine encodes a stream of symbols using a blockwise
  **  Rice encoder.  MaxBits is used to put a limit on the values
  **  the symbols can take.  In particular, the symbols are in the
  **  range 0 to (2^MaxBits)-1.  The special symbol (2^MaxBIts) is
  **  used to signal end-of-file to the decoder.
  **
  *****
  *****/

#define  FLAG_TRUE    1
#define  FLAG_FALSE   0
#define  EOF_SYMBOL  -1
#define  BLOCK_SIZE  16
#define  SYMBOL_SIZE  8

/*****
 *****
 **
 **  In this implementation, the input symbols are grouped in blocks of size
 **  BLOCK_SIZE, which is size 16.
 **
 *****
 *****/
  int i, code_K, End_Input;
  int Block [ BLOCK_SIZE ], Number_of_Symbols, Symbol;
  int  Select_K(), Flush_Bits ();
  BIT_BUFFER       write_buffer;


  End_Input               = FLAG_FALSE;
  write_buffer.bit_number = 0;
  write_buffer.string     = 0;
  MaxBits                 = SYMBOL_SIZE;

  do
    {
     Number_of_Symbols = 0;
     for ( i=0; i < BLOCK_SIZE; i ++ )
        {
          Symbol            = getc ( Input );
          if ( Symbol == EOF_SYMBOL )
            {
              break;
            }
          Number_of_Symbols ++;
          Block [ i ]       = Symbol;
        }
    /*****
     *****
     **  Check whether end_of_file was encountered.
     **
     *****
     *****/
     if ( Number_of_Symbols != BLOCK_SIZE )
       {
          Block [ Number_of_Symbols ] = ( 1 << MaxBits );
          Number_of_Symbols ++;
          End_Input                   = FLAG_TRUE;
       }

    /*****
     *****
     **  Encode the current block.
     **
     *****
     *****/
     code_K = Select_K ( Block, Number_of_Symbols, MaxBits );

     Encode_Block ( Output, Block, Number_of_Symbols, MaxBits, & write_buffer,
                    code_K );
    }
    while ( End_Input == FLAG_FALSE );

    Flush_Bits ( Output, & write_buffer );
    return ( 0 );
}	   

    Encode_Block ( FILE        * Output,
    		   int           Block [],
    		   int    	 Number_of_Symbols,
    		   int    	 MaxBits,
    		   BIT_BUFFER  * write_buffer,
    		   int    	 Code_K
		 )
{
#define CODE_PARAMETER_BITS  3
/******
 ******
 **   Assume that the symbols are 8 bits and that the codes can be
 **   identified by using 3 bits.
 **
 *****
 *****/
 register i, j;
 int	  Symbol, Mask, K_Bits;

 if ( MaxBits < 9 )
     {
      K_Bits = 3;
      Mask   = 7;
     }
 else if ( MaxBits < 17 )
          {
           K_Bits = 4;
           Mask   = 15;
          }
 else
    {
     fprintf ( stderr, "  illegal MaxBits (=%d) in Encode_Block\n", MaxBits );
     exit ( 0 );
    }

 Put_Bits ( Output, K_Bits, write_buffer, Code_K & Mask );

 if ( Code_K != MaxBits - 1 )
   {
     for ( i=0; i < Number_of_Symbols; i ++ )
        {
          Symbol = Block [ i ] >> Code_K;
          /*****
           *****
           **  This loop may appear inefficient but it has the benefit
           **  of not assuming anything about the possible size of
           **  'Symbol'.
           **
           *****
           *****/
           for ( j=0; j < Symbol; j++ )
              Put_Bits ( Output, 1, write_buffer, 1 );

           Put_Bits ( Output, 1, write_buffer, 0 );

          /*****
           *****
           **  Output any trailing raw bits
           **
           *****
           *****/
           if ( Code_K != 0 )
	     {
               Mask =  ( 1 << Code_K ) -1;
               Put_Bits ( Output, Code_K, write_buffer, Block [ i ] & Mask );
	     }
        }
   }
 else
   {
     /*****
      *****
      **  Code_K == MaxBits -1 is the special case we may well output the
      **  symbols as fixed_length codes.  We output symbols in the range
      **  '0' to '(2 ^ MaxBits) -2' as MaxBits long codes.
      **  The symbols '(2 ^ MaxBits ) -1' and '(2 ^ MaxBits )' are output
      **  using MaxBits + 1 bits since we are augmenting the original 
      **  alphabet with a special symbol that will be used to indicate
      **  end_of_file to the decoder.
      **
      *****
      *****/
      Mask = ( 1 << MaxBits ) -1;
      for ( i=0; i < Number_of_Symbols; i ++ )
         {
           if ( Block [ i ] < Mask )
             Put_Bits ( Output, MaxBits, write_buffer, Block [ i ] );
           else
             {
               Put_Bits ( Output, MaxBits, write_buffer, Mask );
               Put_Bits ( Output, 1, write_buffer, 1- ( Block [ i ] & 1 ) );
             }
         }
   }
}		   



   Decode_Stream ( FILE * Input,
                   FILE * Output,
                   int    MaxBits )
{
 /*****
  *****
  **  This routine encodes a stream of symbols using a blockwise
  **  Rice encoder.  MaxBits is used to put a limit on the values
  **  the symbols can take.  In particular, the symbols are in the
  **  range 0 to (2^MaxBits)-1.  The special symbol (2^MaxBIts) is
  **  used to signal end-of-file to the decoder.
  **
  *****
  *****/

#define  FLAG_TRUE    1
#define  FLAG_FALSE   0
#define  EOF_SYMBOL  -1
#define  BLOCK_SIZE  16
#define  SYMBOL_SIZE  8

/*****
 *****
 **
 **  In this implementation, the input symbols are grouped in blocks of size
 **  BLOCK_SIZE, which is size 16.
 **
 *****
 *****/
  int 		   i, code_K, End_Input;
  int 		   Block [ BLOCK_SIZE ], Number_of_Symbols, Symbol;
  int  		   Select_K(), Decode_Block ();
  BIT_BUFFER       read_buffer;


  Number_of_Symbols       = 0;
  End_Input               = FLAG_FALSE;
  read_buffer.bit_number  = 0;
  read_buffer.string      = 0;
  MaxBits                 = SYMBOL_SIZE;

  do
    {
     Decode_Block ( Input, Block, & Number_of_Symbols, MaxBits,
		    & read_buffer, & End_Input );
     
     for ( i=0; i < Number_of_Symbols; i++ )
	{
	   putc ( Block [ i ], Output );
	}
    }
    while ( End_Input == FLAG_FALSE );

    return ( 0 );
}	   

    Decode_Block ( FILE        * Input,
    		   int           Block [],
    		   int         * Number_of_Symbols,
    		   int    	 MaxBits,
    		   BIT_BUFFER  * read_buffer,
    		   int         * End_Flag
		 )
{
#define CODE_PARAMETER_BITS  3
/******
 ******
 **   Assume that the symbols are 8 bits and that the codes can be
 **   identified by using 3 bits.
 **
 *****
 *****/
 int      i, Bits_Read, Mask, count, Limit, K_Bits;
 INT32    code_K, Symbol;

 if ( MaxBits < 9 )
     K_Bits = 3;
 else if ( MaxBits < 17 )
        K_Bits = 4;
 else
    {
     fprintf ( stderr, "  illegal MaxBits (=%d) in Decode_Block\n", MaxBits );
     exit ( 0 );
    }

 (* End_Flag ) = FLAG_FALSE;
 Limit	       = 1 << MaxBits;

 Symbol  = read_buffer-> string;  Mask = read_buffer->bit_number;
 if ( Symbol > ( 1 <<Mask ) -1 ) 
   fprintf ( stderr, "\n Get_Bits about to blow up!\n" );
 if ( K_Bits != Get_Bits ( Input, K_Bits, read_buffer, & code_K) )
    goto premature_EOF;

 if ( code_K < 0 || code_K > 7 )
   {
    fprintf (stderr, "(%d)", code_K);
    fprintf (stderr, "(%d - %d)", Symbol, Mask );
   }
 if ( code_K != MaxBits -1 )
   {
     for ( i=0; i < BLOCK_SIZE; i++ )
        {
          count = 0;

          do 
	    {
	     if ( Get_Bits ( Input, 1, read_buffer, & Symbol) != 1 )
	       goto premature_EOF;

	     if ( Symbol == 1 ) 
		    count ++;
	    } while ( Symbol != 0 );

           if ( code_K != 0 ) 
	     {
	       if ( Get_Bits ( Input, code_K, read_buffer, & Symbol) 
	           != code_K )
	         goto premature_EOF;

	       Block [ i ] = ( count << code_K ) | Symbol;
	     }
           else
	       Block [ i ] = count;
    /*****
     *****
     **  Check whether the symbol we have just read indicates the logical
     **  end-of-file (as opposed to the physical end-of-file)
     **
     *****
     *****/
           if ( Block [ i ] == Limit )
	     {
	       (* Number_of_Symbols ) = i;
	       (* End_Flag )          = FLAG_TRUE;
	       return ( 0 );
	     }
    }
   }
 else
   {  /*****
       *****
       **  We treat code_K =! MaxBits -1 as a special case
       **  where symbols are encoded using MaxBits bits except for the
       **  two symbols (2^ MaxBits)-1 and (2^MaxBits) which are encoded
       **  using (MaxBits+1) bits.  (2^MaxBits) is the special symbol
       **  which can be used either as an escape symbol or to mean
       **  'Logical End of File'
       **
       **
       *****
       *****/
      for ( i=0; i < BLOCK_SIZE; i++ )
         {
            if ( Get_Bits ( Input, MaxBits, read_buffer, &Symbol )
                != MaxBits )
	      goto premature_EOF;

            if ( Symbol == Limit - 1 )
              {
               if ( Get_Bits ( Input, 1, read_buffer, &code_K ) != 1 )
                 goto premature_EOF;

               if ( code_K == 1 )
	         {
	          (* Number_of_Symbols ) = i;
                  (* End_Flag )          = FLAG_TRUE;
	          return ( 0 );
	         }
              }

            Block [ i ] = Symbol;
         }
   }
   /*****
    *****
    **  We have read an entire block of symbols
    **
    *****
    *****/
   (* Number_of_Symbols ) = BLOCK_SIZE;
   return ( 0 );

 premature_EOF:
      (* End_Flag )          = FLAG_TRUE;
      (* Number_of_Symbols ) = 0;
      return ( 1 );
}		   

  main  ()
  /*****
   *****
   **  This is a simple adhoc mainline to test the Rice coding
   **  encoding and decoding procedures.
   **
   *****
   *****/
{
  /*******/
   fprintf ( stderr,  "  Encoding the data stream\n" );
   Encode_Stream ( stdin, stdout, 8 );
   fprintf ( stderr,  "  Finished encoding the data\n" );
  /******/

  /******* 
   fprintf ( stderr,  "  Decoding the data stream\n" );
   Decode_Stream ( stdin, stdout, 8 );
   fprintf ( stderr,  "  Finished decoding the data\n" );
   ******/
}
  /******
   ******
   ||
   ||

od -b test.dat
0000000 061 062 063 064 065 066 067 070 061 062 063 064 065 066 067 012
0000020
> od -b test.out
0000000 264 151 123 251 126 265 173 024 151 123 251 126 265 162 277
0000017
> 
   ||
   ||
   ******
   ******/
