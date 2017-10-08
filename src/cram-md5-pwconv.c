/*
gcc -o cram-md5 `pkg-config --cflags --libs openssl` cram-md5.c && ./cram-md5
*/

#include <string.h>
#include <stdio.h>

#include <openssl/md5.h>
#include <openssl/crypto.h>

#define CDPUT(p, c) (void)({   \
    *(p)++ = (c) & 0xff;       \
    *(p)++ = (c) >> 8 & 0xff;  \
    *(p)++ = (c) >> 16 & 0xff; \
    *(p)++ = (c) >> 24 & 0xff; \
})

static void md5_ctx_to_digest(unsigned char *md, MD5_CTX *ctx) {
    CDPUT(md,ctx->A);
    CDPUT(md,ctx->B);
    CDPUT(md,ctx->C);
    CDPUT(md,ctx->D);
}

static void hexdigest(char *hex, const unsigned char *md) {
	static const char hexchars[17] = "0123456789abcdef";
	int i;
	for (i = 0; i < MD5_DIGEST_LENGTH; ++i) {
		hex[2*i  ] = hexchars[md[i] >> 4];
		hex[2*i+1] = hexchars[md[i] & 0xf];
	}
	hex[2*MD5_DIGEST_LENGTH] = '\0';
}

static void cram_md5(const char *key) {
	MD5_CTX ctx;
	int keylen = strlen(key);
	unsigned char bkey[MD5_CBLOCK] = { 0 };
	unsigned char padkey[MD5_CBLOCK];
	unsigned char digest[MD5_DIGEST_LENGTH];
	unsigned char out[10 + 4 * MD5_DIGEST_LENGTH + 1];
	int i;

	if (keylen < MD5_CBLOCK) {
		memcpy(bkey, key, keylen);
	} else {
		MD5_Init(&ctx);
		MD5_Update(&ctx, key, keylen);
		MD5_Final(bkey, &ctx);
	}

	memcpy(out, "{CRAM-MD5}", 11);

	/* opad */
	for (i = 0; i < MD5_CBLOCK; ++i) padkey[i] = bkey[i] ^ 0x5c;

	MD5_Init(&ctx);
	MD5_Transform(&ctx, padkey);
	md5_ctx_to_digest(digest, &ctx);
	hexdigest(out + 10, digest);

	/* ipad */
	for (i = 0; i < MD5_CBLOCK; ++i) padkey[i] = bkey[i] ^ 0x36;

	MD5_Init(&ctx);
	MD5_Transform(&ctx, padkey);
	md5_ctx_to_digest(digest, &ctx);
	hexdigest(out + 42, digest);

	out[74] = '\0';

	puts(out);

	OPENSSL_cleanse(&ctx,sizeof(ctx));
	OPENSSL_cleanse(&bkey,sizeof(bkey));
	OPENSSL_cleanse(&padkey,sizeof(padkey));
	OPENSSL_cleanse(&digest,sizeof(digest));
	OPENSSL_cleanse(&out,sizeof(out));
}

#include <stdio.h>

int main(int argc, char *argv[]) {
	char *password = NULL;
	size_t pwalloc = 0, pwlen;
	ssize_t r;

    if(argc != 2){
        printf("Usage: %s password\n",argv[0]);
        return 1;
    }

	cram_md5(argv[1]);

	free(password);
	return 0;
}
