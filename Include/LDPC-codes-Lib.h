#pragma once
#ifdef __cplusplus
extern "C"
{
#endif
	int __cdecl make_gen(char* hpck, char* gen, char* inmethod, char* p1, char* p2, char* p3, char* p4);
	int __cdecl alist2pchk(char* alistname, char* pchkname);
	int __cdecl encode(char* pchk_file, char* gen_file, char* source_file, char* encoded_file);
	int __cdecl transmit(char* tfile, char* rfile, char* cseed, char* channel_type, char* p1);
#ifdef __cplusplus
}
#endif