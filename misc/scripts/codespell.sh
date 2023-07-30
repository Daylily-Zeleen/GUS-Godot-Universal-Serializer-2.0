#!/bin/sh
SKIP_LIST="./thirdparty,*.gen.*,*.po,*.pot,package-lock.json,./core/string/locales.h,./DONORS.md,./misc/scripts/codespell.sh"
IGNORE_LIST="ba,childs,curvelinear,expct,fave,findn,gird,inout,lod,nd,numer,ois,ro,statics,te,varn"

codespell -w -q 3 -S "${SKIP_LIST}" -L "${IGNORE_LIST}"
