OPTEE_VERSION ?= "3.6.0"
SRCREV ?= "3.6.0"

SRC_URI_append += "file://0001-gejingquan.patch"

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

