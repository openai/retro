CHECK_FUNCTION_EXISTS(getopt HAVE_GETOPT)
IF(NOT HAVE_GETOPT)
  SET(SRC_EXTRA_FILES ../src/getopt.c)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../src)
ENDIF()

SET(TEST_PROGRAMS
  add_from_filep
  fopen_unchanged
  fseek
  nonrandomopentest
)

SET(GETOPT_USERS
  fread
  tryopen
)

ADD_EXECUTABLE(can_clone_file can_clone_file.c)

SET(ENV{srcdir} ${CMAKE_CURRENT_SOURCE_DIR})
FOREACH(PROGRAM ${TEST_PROGRAMS})
  ADD_EXECUTABLE(${PROGRAM} ${PROGRAM}.c)
  TARGET_LINK_LIBRARIES(${PROGRAM} zip)
ENDFOREACH()

ADD_EXECUTABLE(hole hole.c ${SRC_EXTRA_FILES} source_hole.c)
TARGET_LINK_LIBRARIES(hole zip)

ADD_EXECUTABLE(ziptool_regress ziptool_regress.c ${SRC_EXTRA_FILES} source_hole.c)
TARGET_LINK_LIBRARIES(ziptool_regress zip)

ADD_LIBRARY(malloc MODULE malloc.c)
TARGET_LINK_LIBRARIES(malloc ${CMAKE_DL_LIBS})
ADD_LIBRARY(nonrandomopen MODULE nonrandomopen.c)
TARGET_LINK_LIBRARIES(nonrandomopen ${CMAKE_DL_LIBS})

FOREACH(PROGRAM ${GETOPT_USERS})
  ADD_EXECUTABLE(${PROGRAM} ${PROGRAM}.c ${SRC_EXTRA_FILES})
  TARGET_LINK_LIBRARIES(${PROGRAM} zip)
ENDFOREACH()

# for appveyor builds
#SET(CMAKE_MSVCIDE_RUN_PATH C:/projects/libzip/vstudio/zlib/installed/bin;C:/projects/libzip/vstudio/zlib/installed/lib;C:/projects/libzip/build/lib/Release)

ADD_CUSTOM_TARGET(cleanup
    COMMAND ${CMAKE_COMMAND} -DDIR=${CMAKE_CURRENT_BINARY_DIR} -P ${CMAKE_CURRENT_SOURCE_DIR}/cleanup.cmake
    )

ADD_CUSTOM_TARGET(testinput
  ALL
  VERBATIM
  COMMAND ziptool ${CMAKE_CURRENT_SOURCE_DIR}/manyfiles-zip.zip cat 0 > manyfiles.zip
  COMMAND ziptool ${CMAKE_CURRENT_SOURCE_DIR}/manyfiles-zip.zip cat 1 > manyfiles-133000.zip
  COMMAND ziptool ${CMAKE_CURRENT_SOURCE_DIR}/manyfiles-zip.zip cat 2 > manyfiles-65536.zip
  COMMAND ziptool ${CMAKE_CURRENT_SOURCE_DIR}/manyfiles-zip.zip cat 3 > manyfiles-zip64-modulo.zip
  COMMAND ziptool ${CMAKE_CURRENT_SOURCE_DIR}/manyfiles-zip.zip cat 4 > manyfiles-zip64.zip
  COMMAND ziptool ${CMAKE_CURRENT_SOURCE_DIR}/manyfiles-zip.zip cat 5 > manyfiles-fewer.zip
  COMMAND ziptool ${CMAKE_CURRENT_SOURCE_DIR}/manyfiles-zip.zip cat 6 > manyfiles-more.zip
  COMMAND ziptool ${CMAKE_CURRENT_SOURCE_DIR}/bigzero-zip.zip cat 0 > bigzero.zip
  DEPENDS ziptool ${CMAKE_CURRENT_SOURCE_DIR}/manyfiles-zip.zip ${CMAKE_CURRENT_SOURCE_DIR}/bigzero-zip.zip
)

SET_PROPERTY(DIRECTORY PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
  bigzero.zip
  manyfiles-133000.zip
  manyfiles-65536.zip
  manyfiles-fewer.zip
  manyfiles-more.zip
  manyfiles-zip64-modulo.zip
  manyfiles-zip64.zip
  manyfiles.zip
)

SET(EXTRA_TESTS
	add_dir.test
	add_from_buffer.test
	add_from_file.test
	add_from_file_duplicate.test
	add_from_file_twice_duplicate.test
	add_from_filep.test
	add_from_stdin.test
	add_from_zip_closed.test
	add_from_zip_deflated.test
	add_from_zip_deflated2.test
	add_from_zip_partial_deflated.test
	add_from_zip_partial_stored.test
	add_from_zip_stored.test
	add_stored.test
	add_stored_in_memory.test
	buffer-fragment-read.test
	buffer-fragment-write.test
        clone-buffer-add.test
        clone-buffer-delete.test
        clone-buffer-replace.test
        clone-fs-add.test
        clone-fs-delete.test
        clone-fs-replace.test
	cm-default.test
	count_entries.test
	decrypt-correct-password-aes128.test
	decrypt-correct-password-aes192.test
	decrypt-correct-password-aes256.test
	decrypt-correct-password-pkware.test
	decrypt-no-password-aes256.test
	decrypt-wrong-password-aes128.test
	decrypt-wrong-password-aes192.test
	decrypt-wrong-password-aes256.test
	decrypt-wrong-password-pkware.test
	delete_add_same.test
	delete_invalid.test
	delete_last.test
	delete_multiple_last.test
	delete_multiple_partial.test
	delete_renamed_rename.test
	encrypt.test
	encryption-nonrandom-aes128.test
	encryption-nonrandom-aes192.test
	encryption-nonrandom-aes256.test
	encryption-remove.test
	extra_add.test
	extra_add_multiple.test
	extra_count.test
	extra_count_by_id.test
	extra_count_ignore_zip64.test
	extra_delete.test
	extra_delete_by_id.test
	extra_field_align.test
	extra_get.test
	extra_get_by_id.test
	extra_set.test
	extra_set_modify_c.test
	extra_set_modify_l.test
	fdopen_ok.test
	file_comment_encmismatch.test
	fopen_unchanged.test
	fread.test
	fseek_deflated.test
	fseek_fail.test
	fseek_ok.test
	get_comment.test
	junk_at_end.test
	junk_at_start.test
	name_locate.test
	open_cons_extrabytes.test
	open_empty.test
	open_empty_2.test
	open_extrabytes.test
	open_file_count.test
	open_filename_duplicate.test
	open_filename_duplicate_consistency.test
	open_filename_duplicate_empty.test
	open_filename_duplicate_empty_consistency.test
	open_filename_empty.test
	open_incons.test
	open_many_fail.test
	open_many_ok.test
	open_multidisk.test
	open_new_but_exists.test
	open_new_ok.test
	open_nonarchive.test
	open_nosuchfile.test
	open_ok.test
	open_too_short.test
	open_truncate.test
	open_zip64_3mf.test
	open_zip64_ok.test
	preload.test
	progress.test
	rename_ascii.test
	rename_cp437.test
	rename_deleted.test
	rename_fail.test
	rename_ok.test
	rename_utf8.test
	rename_utf8_encmismatch.test
	set_comment_all.test
	set_comment_localonly.test
	set_comment_removeglobal.test
	set_comment_revert.test
	set_compression_bzip2_to_deflate.test
	set_compression_deflate_to_bzip2.test
	set_compression_deflate_to_deflate.test
	set_compression_deflate_to_store.test
	set_compression_store_to_bzip2.test
	set_compression_store_to_deflate.test
	set_compression_store_to_store.test
	set_compression_unknown.test
	stat_index_cp437_guess.test
	stat_index_cp437_raw.test
	stat_index_cp437_strict.test
	stat_index_fileorder.test
	stat_index_streamed.test
	stat_index_streamed_zip64.test
	stat_index_utf8_guess.test
	stat_index_utf8_raw.test
	stat_index_utf8_strict.test
	stat_index_utf8_unmarked_strict.test
	stat_index_zip64.test
	utf-8-standardization.test
	zip-in-archive-comment.test
	zip64_creation.test
	zip64_stored_creation.test
)

FOREACH(CASE ${EXTRA_TESTS})
  ADD_TEST(${CASE} perl ${CMAKE_BINARY_DIR}/regress/runtest ${CMAKE_CURRENT_SOURCE_DIR}/${CASE})
  SET_TESTS_PROPERTIES(${CASE} PROPERTIES SKIP_RETURN_CODE 77)
ENDFOREACH()

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../lib
	${CMAKE_CURRENT_SOURCE_DIR}/../src
	${CMAKE_CURRENT_BINARY_DIR}/..)

ADD_CUSTOM_TARGET(check COMMAND ${CMAKE_CTEST_COMMAND})
