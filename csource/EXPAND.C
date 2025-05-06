void    expand_to_16_chars(char *name)
        {
        char *p;
        char tmp[17];
        int  i;

        memset(tmp, ' ', 15);
        p = name;
        i = 0;
        while (i < 15 && *p)
            {
            tmp[i] = *p;
            i++;
            p++;
            }
        tmp[15] = '\0';
        strcpy(name, tmp);
        }
