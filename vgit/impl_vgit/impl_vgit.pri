#########################################################################################
##
##  VLIBS codebase, NIIAS
##
##  GNU Lesser General Public License Usage
##  This file may be used under the terms of the GNU Lesser General Public License
##  version 3 as published by the Free Software Foundation and appearing in the file
##  LICENSE.LGPL3 included in the packaging of this file. Please review the following
##  information to ensure the GNU Lesser General Public License version 3 requirements
##  will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
#########################################################################################
# vgit_impl/vgit_impl.pri

#========================================================================================
#
#   http://blog.mgsxx.com/?p=2140
#
#   Это система автоматического подтягивания в плюсовый код информации о текущем коммите.
#
#   Как это реализуется надо обязательно и подробно описать, смысл такой, чтобы скриптами
#   перейти в папку с исходниками, награбить информацию во флаги компиляции и
#   макросами вытянуть их до состояния объектов C++. Короче, магия, в основном, черная.
#
#   UPD 17-08-2018
#   Решена проблема "оторванной головы". Когда реп находится в статусе Detached HEAD,
#   команда git branch выдает "* (HEAD detached at 27130b6)".
#   В результате, awk вытягивает "(HEAD". Скобочка, в итоге, делает больно препроцессору.
#   Юра написал команду sed, чтобы избавиться от этой самой скобочки.
#
#   UPD 2019-02-06  by Elapidae
#   Вся механика перенесена в этот файл (vgit_impl.pri) чтобы случайно не затереть.
#
#========================================================================================

isEmpty(qi_vgit_impl) {
    # -----------------------------------------------------------------------------------
    qi_vgit_impl = 1;
    isEmpty(qi_not_print_pri_messages): message("=== vgit (impl) appended ===")

    # MAIN_DIR symbol control -----------------------------------------------------------
    isEmpty(MAIN_DIR): error( "vgit: Please, specify MAIN_DIR=\$\$PWD in your .pro \
                               file BEFORE including vgit.pri")

    # -----------------------------------------------------------------------------------
    #   Основная машинерия с преферансом, бубном и танцами. Страшновато представить как
    #   все вот это надо будет еще и под виндой поддерживать...
    VGIT_HASH           = "$$system(cd \"$$system_path($$MAIN_DIR)\"  &&  \
                           git log -n 1 --pretty=format:\"%H\")"

    VGIT_REVCOUNT       = "$$system(cd \"$$system_path($$MAIN_DIR)\"  &&  \
                           git rev-list HEAD --count)"

    VGIT_BRANCH         = "$$system(cd \"$$system_path($$MAIN_DIR)\"  &&  \
                           git symbolic-ref --short HEAD)"

    VGIT_AUTHOR_NAME    = "$$system(cd \"$$system_path($$MAIN_DIR)\"  &&  \
                           git log -n 1 --pretty=format:\"%an\")"

    VGIT_AUTHOR_EMAIL   = "$$system(cd \"$$system_path($$MAIN_DIR)\"  &&  \
                           git log -n 1 --pretty=format:\"%ae\")"

    VGIT_DATE           = "$$system(cd \"$$system_path($$MAIN_DIR)\"  &&  \
                           git log -n 1 --pretty=format:\"%ci\")"

    # -----------------------------------------------------------------------------------

    DEFINES +=         VGIT_HASH_ELPD=\"$${VGIT_HASH}\"
    DEFINES +=     VGIT_REVCOUNT_ELPD=\"$${VGIT_REVCOUNT}\"
    DEFINES +=       VGIT_BRANCH_ELPD=\"$${VGIT_BRANCH}\"
    DEFINES +=  VGIT_AUTHOR_NAME_ELPD=\"$${VGIT_AUTHOR_NAME}\"
    DEFINES += VGIT_AUTHOR_EMAIL_ELPD=\"$${VGIT_AUTHOR_EMAIL}\"
    DEFINES +=         VGIT_DATE_ELPD=\"$${VGIT_DATE}\"

    # -----------------------------------------------------------------------------------

    message(">>> Current git hash: $${VGIT_HASH}, \
                         revcount: $${VGIT_REVCOUNT}, \
                           branch: $${VGIT_BRANCH}, \
                      author-name: $${VGIT_AUTHOR_NAME}, \
                     author-email: $${VGIT_AUTHOR_EMAIL}, \
                             date: $${VGIT_DATE}\
                             ")

    # -----------------------------------------------------------------------------------

    # Удаление объектных файлов и Makefile необходимо, чтобы инкрементальная сборка
    # обязательно пересобрала всех участников этой пирушки.
    VMAIN_O = $$system_path($$OUT_PWD/$$OBJECTS_DIR/main.o)
    VGIT_O  = $$system_path($$OUT_PWD/$$OBJECTS_DIR/vgit.o)
    VMakeFileForRemove = $$system_path($$OUT_PWD/Makefile)
    QMAKE_POST_LINK  += \
                        $(DEL_FILE) $${VMAIN_O} && \
                        $(DEL_FILE) $${VGIT_O}  && \
                        $(DEL_FILE) $${VMakeFileForRemove}

    # -----------------------------------------------------------------------------------
} # ifndef qi_vgit_impl
# vgit_impl/vgit_impl.pri
#========================================================================================
