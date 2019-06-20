TEMPLATE = subdirs

SUBDIRS += src

#SUBDIRS += demo
#demo.depends = src

SUBDIRS += vocabularycreator
vocabularycreator.depends = src
