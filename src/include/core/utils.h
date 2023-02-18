#ifndef SRC_INCLUDE_CORE_UTILS_H
#define SRC_INCLUDE_CORE_UTILS_H


#define NON_COPYABLE(class_name)           \
  class_name(const class_name &) = delete; \
  class_name &operator=(const class_name &) = delete

#define NON_MOVEABLE(class_name)      \
  class_name(class_name &&) = delete; \
  class_name &operator=(class_name &&) = delete

#define NON_COPYABLE_AND_MOVEABLE(class_name) \
  NON_COPYABLE(class_name);                   \
  NON_MOVEABLE(class_name)



#endif