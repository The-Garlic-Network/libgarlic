# libgarlic

<img src="https://github.com/mrrva/media/blob/master/libgarlic1.png" align="right"
     alt="Storeon logo by Anton Lovchikov" width="160" height="160">

The library for building your own project based on **The Garlic Network**.

* **All logic is already written.** You don't need to understand how network
works, library will do everything for you.
* **Easy to understand.** There are just 10 API function for usage.
* **Written on C++.** It Means the library can be adapted to any
programmin language.

Read more about libgarlic and The Garlic Network on the web-site https://garlic.network.

```C
#include <stdio.h>
#include <stdlib.h>
#include "libgarlic.h"
/** 
* Example of initialization and stopping of the libgarlic.
*/
int main() {
  // Start threads.
  the_garlic_network();
  // Stop threads.
  tgn_stop();
  
  return 0;
}
```

```C
#include <stdio.h>
#include <stdlib.h>
#include "libgarlic.h"
/** 
* Example of getting own public hash (it's like an ip address in the internet).
*/
int main() {
  const char *hash;

  the_garlic_network();
  // Getting and printing.
  hash = tgn_myhash();
  printf("My hash is - %s\n", hash);
  
  free(hash);
  tgn_stop();
  
  return 0;
}
```
