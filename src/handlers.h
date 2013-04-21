typedef void (*mkfile_handler)(struct Entry*, struct Entry*);
#define HANDLER(x) void x(struct Entry* index, struct Entry* content)

#define HANDLERS()\
H(packer_mkfile) H(packer_mkdir)

#define H(a) extern HANDLER(a);
HANDLERS();
#undef H

#define H(a) a,
mkfile_handler handlers[] = {HANDLERS()};
#undef H

HANDLER(packer_mkfile){
    char s[index->size];
    FILE *fp = fopen(p_get_filename(s, index->content), "wb");
    fwrite(content->content, 1, content->size, fp);
    fclose(fp);
}

HANDLER(packer_mkdir){
    chdir(cwd);
    mkdir(index->content);
    chdir(index->content);
}


