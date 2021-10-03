             ______       _      __
            / __/ /  ____(_)__  / /__
           _\ \/ _ \/ __/ / _ \/  '_/
          /___/_//_/_/ /_/_//_/_/\_\ 
          version: 1.0.0 (beta)
------------------------------------------------------
    A program to short links using command line
        Download :-
          1. Debian Package *.deb* : https://github.com/Lil-Nickel/shrink/releases/download/linux/shrink-1.0.0-beta_amd64.deb
          2. Linux Portable *.tar.xz* :https://github.com/Lil-Nickel/shrink/releases/download/linux/shrink-1.0.0-beta_amd64.tar.xz
          
        Usage :-
          Required options:
            -u, --uri arg  long url
            
          Optinal options:
            -a, --alias arg    short alias (if service support)
            -s, --service arg  shortening service, (is.gd, bit.ly, git.io, 0i.is)
            -f, --format arg   output format, (text, cli, json, xml)
            -h, --help         this help message
            -v, --version      print shrink version
            
          Example:
            user@host:~$ ./shrink -u https://libyana.ly -f xml -s bit.ly
            
            <?xml version="1.0" encoding="UTF-8" ?>
            <root>
              <err>fasle</err>
              <code>0</code>
              <result>https://bit.ly/2U6Mssn</result>
            </root>

          Made with ❤ & hand craft 
