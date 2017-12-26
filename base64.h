#ifndef __BASE64_H__2017_12_04__
#define __BASE64_H__2017_12_04__

enum { BASE64_OK = 0, BASE64_INVALID };

#define BASE64_PADDING			'='
#define BASE64_ENCODE_SIZE(s)	(((s) + 2) / 3 * 4)
#define BASE64_DECODE_SIZE(s)	(((s)) / 4 * 3)

int base64_encode(const unsigned char *in, unsigned int inlen, char *out, unsigned int *outlen);
int base64_decode(const char *in, unsigned int inlen, unsigned char *out, unsigned int *outlen);

#endif /* __BASE64_H__2017_12_04__ */

