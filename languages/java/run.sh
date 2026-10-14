#!/bin/sh
exec java -Xshare:auto -XX:TieredStopAtLevel=1 -cp /app FiscalCode
