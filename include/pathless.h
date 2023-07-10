/*
* Pathless is a which implements various file I/O operations.
* 
* 
* 
* Named after the Pathless (https://store.steampowered.com/app/1492680/The_Pathless/)
* */

#ifndef PATHLESS_H
#define PATHLESS_H

#include <delight.h>

#ifndef PATHLESS_INTERNAL

typedef void IterationFile;
typedef void Directory;
typedef void ConfigFile;

#endif

/// @brief Begins iterating across directory, listing every file or folder as specified by these parameters. The iteration file starts before the first element, so pathless_iterate_next should be called in order to move it to the next element.
/// @param path The relative or absolute path to the directory we wish to open. May end with /.
/// @param recursive Whether or not we want to iterate into subdirectories.
/// @param allowDirs Whether or not current should also return directories.
/// @param onlyExtensions Files with which extensions should be returned. If all files should be permitted, pass {""}.
/// @param extensionCount The number of file extensions in the onlyExtensions array. 0 means that no files will be permitted.
/// @return An object which allows iterating over file.
IterationFile* 					pathless_iterate_begin(const char* const path, bool recursive, bool allowDirs, const char** onlyExtensions, u32 extensionCount);

/// @brief End iteration across directory, freeing this iteration file.
/// @param file Which iteration file should be ended. 
void 							pathless_iterate_end(IterationFile* file);

/// @brief Returns the current file this IterationFile is pointing to. This function has no effect on the state of the iteration file.
/// @param file The current IterationFile.
/// @return The file it is pointing to.
const char* const				pathless_iterate_get_current(IterationFile* file);

/// @brief If there is another file this iteration file could iterate to, returns true and updates the return of pathless_iterate_get_current. Otherwise returns false.
/// @param file The current IterationFile.
/// @return True if there are more files to iterate to.
bool 							pathless_iterate_next(IterationFile* file);

/// @brief Opens the directory pointed to by path. ALL filesystem operations related to opening a directory are performed by this function.
/// @param path The relative or absolute path to a directory.
/// @return The directory reference object.
Directory* 						pathless_directory_open(const char* const path);

/// @brief Closes the opened directory, freeing associated memory.
/// @param directory The directory we wish to close.
void 							pathless_directory_close(Directory* directory);

// should probably add a FileData thing instead of this to allow more easily reading stuff. probably adding it in the next general IO push

/// @brief Returns an array of fileCount files, which are directly contained within this directory. The paths are of the format "directoryPath/file", where directoryPath is the path given to pathless_directory_open and file is the full file extension.
/// @param directory The directory we want to open.
/// @param fileCount The number of files in the returned an array.
/// @return The internal array of files this directory contains. This should NOT be freed.
const char* const * const		pathless_directory_ls(Directory* directory, u32* fileCount);

/// @brief Opens file at path as text file and copies its contents into the returned character buffer, writing that string's length to length if a valid pointer was passed.
/// @param path The path to the text file we wish to read.
/// @param length The length of the text file, assuming it was read.
/// @return A null terminated character buffer containing the contents of the file at path, or NULL if there was an issue opening it.
char*							pathless_file_read_as_text(const char* const path, u64* length);

/// @brief Creates a text file and copies buffer into it, up to length.
/// @param path The path at which it creates the new file. 
/// @param buffer The buffer we wish to copy.
/// @param length The buffer's length. If 0, calculates the length of the buffer, presupposing it's null terminated.
void							pathless_file_write_as_text(const char* const path, char* buffer, u64 length);


/// @brief Creates new, empty, config file object. 
/// @return A pointer to the newly created config file object.
ConfigFile*						pathless_config_file_new(void);

/// @brief Frees the given config file, alongside all its internal categories, keys and values.
/// @param file The file we wish to free.
void							pathless_config_file_free(ConfigFile* file);

/// @brief Clears all keys and categories from config file without deleting it.
/// @param file The file we wish to clear.
void							pathless_config_file_clear(ConfigFile* file);

/// @brief Saves the current contents of the config file to a specified location.
/// @param file The file we wish to save.
/// @param path The path to where we wish to save this config file.
void							pathless_config_file_save(ConfigFile* file, const char* const path);

/// @brief Loads contents from config file at path to file, clearing the file first.
/// @param file The file we wish to load to.
/// @param path The path we wish to load from.
void							pathless_config_file_load(ConfigFile* file, const char* const path);

/// @brief Saves given string to category category with key file on file. The passed string is copied and can be changed after.
/// @param file The file we wish to save a string to.
/// @param category The category we wish to save a string to.
/// @param key The key under which we wish to save a string.
/// @param value The value we wish to save.
void							pathless_config_file_save_string(ConfigFile* file, const char* const category, const char* const key, const char* const value);

/// @brief Saves given int to category category with key file on file.
/// @param file The file we wish to save an int to.
/// @param category The category we wish to save an int to.
/// @param key The key under which we wish to save an int.
/// @param value The value we wish to save.
void							pathless_config_file_save_int(ConfigFile* file, const char* const category, const char* const key, int value);

/// @brief Saves given bool to category category with key file on file.
/// @param file The file we wish to save a bool to.
/// @param category The category we wish to save a bool to.
/// @param key The key under which we wish to save a bool.
/// @param value The value we wish to save.
void							pathless_config_file_save_bool(ConfigFile* file, const char* const category, const char* const key, bool value);

/// @brief Attempts to load a string from file file, under category category, key key.
/// @param file The file we wish to load from.
/// @param category The category we wish to load from.
/// @param key The key we wish to load from.
/// @param value The read value, assuming it exists.
/// @return Returns true if the config file contains a valid string under the provided path.
bool							pathless_config_file_try_load_string(ConfigFile* file, const char* const category, const char* const key, char** value);

/// @brief Attempts to load an int from file file, under category category, key key.
/// @param file The file we wish to load from.
/// @param category The category we wish to load from.
/// @param key The key we wish to load from.
/// @param value The read value, assuming it's valid and exists.
/// @return Returns true if the config file contains a valid int under the provided path.
bool							pathless_config_file_try_load_int(ConfigFile* file, const char* const category, const char* const key, i32* value);

/// @brief Attempts to load a bool from file file, under category category, key key.
/// @param file The file we wish to load from.
/// @param category The category we wish to load from.
/// @param key The key we wish to load from.
/// @param value The read value, assuming it's valid and exists.
/// @return Returns true if the config file contains a valid bool under the provided path.
bool							pathless_config_file_try_load_bool(ConfigFile* file, const char* const category, const char* const key, bool* value);

/// @brief Gets all categories contained in this file.
/// @param file The file from which we wish to get all categories.
/// @param count Contains the number of categories contained by this file.
/// @return The first element in a string array containing all categories.
char**							pathless_config_file_get_categories(ConfigFile* file, u32* count);

/// @brief Gets all keys in a category contained in this file.
/// @param file The file from which we wish to get all keys.
/// @param category The category from which we wish to get all keys.
/// @param count Contains the number of keys this category has. 
/// @return The first element in a string array containing all keys.
char**							pathless_config_file_get_keys(ConfigFile* file, const char* const category, u32* count);

/// @brief Removes specified key from this config file.
/// @param file The file we wish to remove a key from.
/// @param category The category from which we wish to remove a key.
/// @param key The key we wish to remove.
void 							pathless_config_file_remove_key(ConfigFile* file, const char* const category, const char* const key);

/// @brief Removes specified category from this config file. This removes all keys associated with it.
/// @param file The file we wish to remove a category.
/// @param category The category we wish to remove.
void							pathless_config_file_remove_category(ConfigFile* file, const char* const category);

#endif