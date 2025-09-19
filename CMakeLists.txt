cmake_minimum_required(VERSION 3.10)
project(TP_DCT C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

# ---- Sources principales ----
set(OBJS
    bit.c bitstream.c bits.c entier.c sf.c matrice.c
    dct.c psycho.c rle.c image.c jpg.c ondelette.c
)

# ---- Utilitaires ----
set(UTILITAIRES
    eprintf.c intstream.c filtres.c
)

# ---- Objets de test (_tst.c à compiler) ----
set(OBJSTST)
foreach(src ${OBJS})
    string(REPLACE ".c" "_tst.c" tst ${src})
    list(APPEND OBJSTST ${tst})
endforeach()

# ---- Executable de tests ----
add_executable(tests
    tests.c
    ${OBJS}
    ${OBJSTST}
    ${UTILITAIRES}
)

# Lien avec la libm
target_link_libraries(tests m)

# ---- Règle "run" équivalente ----
# Exécute ./tests dans le répertoire parent des sources
add_custom_target(run
    COMMAND ${CMAKE_COMMAND} -E echo "Vérification de l'environnement..."
    COMMAND ${CMAKE_COMMAND} -E env bash -c "if [ ! -h dct ]; then echo 'Lien symbolique dct manquant'; fi"
    COMMAND ${CMAKE_COMMAND} -E env bash -c "if [ ! -d DONNEES ]; then echo 'Répertoire DONNEES manquant'; fi"
    COMMAND ${CMAKE_COMMAND} -E env bash -c "if [ $$(id -u) = 0 ]; then echo 'NE PAS TRAVAILLER EN ROOT'; exit 1; fi"
    COMMAND ./tests
    DEPENDS tests
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# ---- Nettoyage custom ----
add_custom_target(extra_clean
    COMMAND ${CMAKE_COMMAND} -E rm -f xxx* TAGS
)
