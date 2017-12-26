#include "base64.h"

/* BASE64 encode table 64 */
static const unsigned char base64en[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', '+', '/',
};

/* BASE64 decode table 256, character '=' is 0xFE, 0xFF in unused character */
static const unsigned char base64de[] = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E, 0xFF, 0xFF, 0xFF, 0x3F, 
	0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 
	0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 
	0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 
	0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

int base64_encode(const unsigned char *in, unsigned int inlen, char *out, unsigned int *outlen)
{
	unsigned int i, n = inlen / 3 * 3;

	if(*outlen < BASE64_ENCODE_SIZE(inlen))
	{
		return BASE64_INVALID;
	}

	*outlen = BASE64_ENCODE_SIZE(inlen);

	for(i = 0; i < n; i += 3, in += 3, out += 4)
	{
		out[0] = base64en[(in[0] >> 2) & 0x3F];
		out[1] = base64en[((in[0] & 0x3) << 4) + ((in[1] >> 4) & 0xF)];
		out[2] = base64en[((in[1] & 0xF) << 2) + ((in[2] >> 6) & 0x3)];
		out[3] = base64en[in[2] & 0x3F];
	}

	switch(inlen - n)	
	{
		case 1:
		{
			out[0] = base64en[(in[0] >> 2) & 0x3F];
			out[1] = base64en[(in[0] & 0x3) << 4];
			out[2] = BASE64_PADDING;
			out[3] = BASE64_PADDING;
			break;
		}
		case 2:
		{
			out[0] = base64en[(in[0] >> 2) & 0x3F];
			out[1] = base64en[((in[0] & 0x3) << 4) + ((in[1] >> 4) & 0xF)];
			out[2] = base64en[(in[1] & 0xF) << 2];
			out[3] = BASE64_PADDING;
			break;
		}
	}

	return BASE64_OK;
}

int base64_decode(const char *in, unsigned int inlen, unsigned char *out, unsigned int *outlen)
{
	unsigned char B1, B2, B3, B4, *orgout = out;
	unsigned int  i, n = inlen / 4 * 4 - 4, ret = BASE64_OK;

	if(*outlen < BASE64_DECODE_SIZE(inlen))
	{
		return BASE64_INVALID;
	}

	for(i = 0; i < n; i += 4, in += 4, out += 3)
	{
		B1 = base64de[in[0]];
		B2 = base64de[in[1]];
		B3 = base64de[in[2]];
		B4 = base64de[in[3]];

		// invalid base64 data
		if(B1 >= 0xFE || B2 >= 0xFE || B3 >= 0xFE || B4 >= 0xFE)
		{
			*outlen = (unsigned int)(out - orgout);
			return BASE64_INVALID;
		}

		out[0] = (B1 << 2) | ((B2 >> 4) & 0x3);
		out[1] = ((B2 & 0xF) << 4) | ((B3 >> 2) & 0xF);
		out[2] = ((B3 & 0x3) << 6) | B4;
	}

	*outlen = (unsigned int)(out - orgout);

	// last 4 characters
	do 
	{
		B1 = base64de[in[0]]; B2 = base64de[in[1]];

		if(B1 >= 0xFE || B2 >= 0xFE)
		{
			ret = BASE64_INVALID;
			break;
		}

		*outlen += 1;
		out[0] = (B1 << 2) | ((B2 >> 4) & 0x3);

		if((B3 = base64de[in[2]]) >= 0xFE) // 2 '=' must be one byte
		{
			if(B3 == 0xFF)
			{
				ret = BASE64_INVALID;
			}
			break;
		}

		*outlen += 1;
		out[1] = ((B2 & 0xF) << 4) | ((B3 >> 2) & 0xF);

		if((B4 = base64de[in[3]]) >= 0xFE) // 1 '=' must be two byte
		{
			if(B4 == 0xFF)
			{
				ret = BASE64_INVALID;
			}
			break;
		}

		*outlen += 1;
		out[2] = ((B3 & 0x3) << 6) | B4;

	} while (0);

	return ret;
}