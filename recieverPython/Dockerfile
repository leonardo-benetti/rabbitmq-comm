FROM python:3.8-slim-buster

WORKDIR /app

COPY ./app/ /app

RUN pip install -r /app/requirements.txt

EXPOSE 7000

ENTRYPOINT ["python","/app/reciever.py"]
# CMD tail -f /dev/null
