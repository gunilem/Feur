#pragma once




namespace GUFeur {
	#define GUF_DELETE_COPY_CONSTRUCTORS(classname) classname(classname&&) = delete; \
													classname(const classname&) = delete; \
													classname& operator=(classname&&) = delete; \
													classname& operator=(const classname&) = delete;
}