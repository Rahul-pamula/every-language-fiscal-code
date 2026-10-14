# Python

Implementation of the Italian Fiscal Code in Python 3, using only the standard library.

- Source: [`fiscal_code.py`](fiscal_code.py)
- Runtime: `python:3.13-alpine` (see [`Dockerfile`](Dockerfile))

```bash
./elc test python
echo '{"surname":"ROSSI","name":"MARIO","birth_date":"1980-01-01","sex":"M","birthplace_code":"H501"}' | ./elc run python
```
