SRC_URI += "file://bsp.cfg \
		   "

 
SRC_URI_append += "file://kernel_optee.cfg"

SRC_URI_append += "file://0001-gejingquan.patch" 

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
