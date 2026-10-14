# PHP

Implementation of the Italian Fiscal Code in PHP 8, using only core functions.

- Source: [`fiscal_code.php`](fiscal_code.php)
- Runtime: `php:8.3-cli-alpine` (see [`Dockerfile`](Dockerfile))

```bash
./elc test php
echo '{"surname":"ROSSI","name":"MARIO","birth_date":"1980-01-01","sex":"M","birthplace_code":"H501"}' | ./elc run php
```
