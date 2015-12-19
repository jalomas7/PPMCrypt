# PPMCrypt
command line program that can perform a basic OR-based stegnographic encryption and decryption using .ppm image files.
All source files are written in C++, and an example ppm image is included.

The security of this program comes from both the key used, and the image itself. If one key is used to encrypt a message in one image, that same key will not decrypt the same message in another image. since each key is represented as an integer value, each encryption per unique image has 2^32 possible keys.
