# libgarlic

<img src="https://github.com/mrrva/media/blob/master/libgarlic1.png" align="right"
     alt="Storeon logo by Anton Lovchikov" width="160" height="160">

The library for building your own project based on **The Garlic Network**.

* **All logic is already written.** You don't need to understand how network
works, library will do everything for you.
* **Written on C++.** It Means the library can be adapted to any
programmin language.
* **Easy to understand.** There are just 10 API function for usage.

Read more about libgarlic and The Garlic Network on the web-site https://garlic.network.

Dependencies: `libsqlite3` `libsodium`.

---

Full examples of usage you will get in `example` folder.

```C
/** 
* Example of initialization and stopping of the libgarlic.
*/
#include <stdio.h>
#include <stdlib.h>
#include "libgarlic.h"

int main() {
  // Start threads.
  the_garlic_network();
  // Stop threads.
  tgn_stop();
  
  return 0;
}
```

```C
/** 
* Example of getting own public hash (it's like an ip address in the internet).
*/
#include <stdio.h>
#include <stdlib.h>
#include "libgarlic.h"

int main() {
  const char *hash;

  the_garlic_network();
 
  hash = tgn_myhash(); // Getting the hash.
  printf("My hash is - %s\n", hash);
  
  free(hash);
  tgn_stop();
  
  return 0;
}
```

```C
/** 
* Example of callback function setting. Function will be called when was
* received new package.
*/
#include <stdio.h>
#include <stdlib.h>
#include "libgarlic.h"

// @from - Sender (hash).
// @pack - Byte array.
// @len - length of byte array.
void callback_fn(char *from, unsigned char *pack, size_t len) {
     printf("Was received new package from %s\n", from);
     // . . . 
     free(from);
     free(pack);
}

int main() {
     the_garlic_network();
     tgn_callback(callback_fn); // Callback function.
     // . . .
     tgn_stop();
     return 0;
}
```

```C
/** 
* Example of tgn server. When was received new package server will send
* response.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libgarlic.h"

void callback_fn(char *from, unsigned char *pack, size_t len) {
     // Convert char pointer to const char pointer.
     const char *hash = (const char *)from;
     char *text = "Hello dude!";
     // Length of *text.
     size_t length = 12;
     // Byte array,
     unsigned char buffer[length];
     
     // Copy text to byte array.
     memcpy(buffer, text, length);
     // Send package back.
     tgn_send_msg(hash, length, buffer);

     free(from);
     free(pack);
}

int main() {
     the_garlic_network();
     tgn_callback(callback_fn);
     
     while (tgn_is_working()) {
          // . . .
     }
     
     tgn_stop();
     return 0;
}
```

```C
/** 
* Example of getting sent package status and try to resend package if
* status code is ERROR_ROUTE or ERROR_TARGET.
*/
#include <stdio.h>
#include <stdlib.h>
#include "libgarlic.h"

int main() {
     // Vars and pointers.
     enum tgn_status status;
     size_t id;
     // ...
     
     the_garlic_network();
     // Send message and get id of pack.
     id = tgn_send_msg(hash, 10, buffer);
     
     // ...
     
     // Getting status.
     status = tgn_pack_status(id);
     
     switch (status) {
     // Errors handler.
     case ERROR_ROUTE:
     case ERROR_TARGET:
          // Error detected. Try to send pack again.
          // Package resending by id.
          tgn_resend_package(id);
          break;
          
     case GOOD_TARGET:
          // Package was received by target user.
          break;
          
     case REQUEST_FIND:
     case GOOD_SERVER:
          // Just wait a little bit. Package during delivery.
          break;
     }
     
     tgn_stop();
     return 0;
}
```
