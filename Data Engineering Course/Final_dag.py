# step 1 - import modules
import requests
import json

from airflow import DAG
from datetime import datetime
from datetime import date
# Operators; we need this to operate!
from airflow.operators.python_operator import PythonOperator

import pandas as pd

import tweepy
from textblob import TextBlob
import sys
import tweepy
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import os
import nltk
import pycountry
import re
import string
from wordcloud import WordCloud, STOPWORDS
from PIL import Image
from nltk.sentiment.vader import SentimentIntensityAnalyzer
from langdetect import detect
from nltk.stem import SnowballStemmer
from nltk.sentiment.vader import SentimentIntensityAnalyzer
from sklearn.feature_extraction.text import CountVectorizer

import datetime as dt


# step 2 - define default args
# These args will get passed on to each operator
# You can override them on a per-task basis during operator initialization
default_args = {
    'owner': 'airflow',
    'depends_on_past': False,
    'start_date': datetime(2021, 1, 8)
}


# step 3 - instantiate DAG
dag = DAG(
    'm3-DAG',
    default_args=default_args,
    description='Fetch covid data from API',
    schedule_interval='@daily',
    catchup=True
)


def CollectTweets(**kwargs):
    consumer_key = 'E74X2MrmSCf98mOlxHmN7otzZ'
    consumer_secret = 'ivOjTqh7jKtOS4DlSGAMS6tyWEdpRRDohDG6LbfAbvMSEZEuj0'
    access_token = '1638348272-QGHBCBWy0E1kOcrnmssRTjfQlBxo6Tq2sn75OuQ'
    access_token_secret = 'f5aish23ZmYbCZd3SL8iQTQdiYhb2I5R7bPgEDnAdn41B'

    # access to the Twitter API
    auth = tweepy.OAuthHandler(consumer_key, consumer_secret)
    auth.set_access_token(access_token, access_token_secret)

    api = tweepy.API(auth)
    # the newest 20 tweets from Norway country
    places = api.geo_search(query="Norway", granularity="country")
    place_id = places[0].id

    search_results = api.search(q="place:%s -filter:retweets" %
                                place_id, tweet_mode='extended', count=20, lang='en')

    tweets_for_Norway = [[tweet.id_str, tweet.created_at, tweet.full_text, tweet.lang,
                          tweet.place.country] for tweet in search_results]  # CSV file created

    # the newest 20 tweets from Burundi country
    places = api.geo_search(query="Burundi", granularity="country")
    place_id = places[0].id

    search_results = api.search(
        q="place:%s -filter:retweets" % place_id, tweet_mode='extended', count=20)

    tweets_for_Burundi = [[tweet.id_str, tweet.created_at, tweet.full_text,
                           tweet.lang, tweet.place.country] for tweet in search_results]  # CSV file created

    return [tweets_for_Norway, tweets_for_Burundi]


def percentage(part, whole):
    return 100 * float(part)/float(whole)


def tweets_happiness_score(tweets):
    nltk.download('vader_lexicon')
    noOfTweet = len(tweets)
    positive = 0
    negative = 0
    neutral = 0
    polarity = 0
    tweet_list = []
    neutral_list = []
    negative_list = []
    positive_list = []
    for tweet in tweets:
        tweet_list.append(tweet)
        analysis = TextBlob(tweet)
        score = SentimentIntensityAnalyzer().polarity_scores(tweet)
        neg = score['neg']
        neu = score['neu']
        pos = score['pos']
        comp = score['compound']
        polarity += analysis.sentiment.polarity

        if neg > pos:
            negative_list.append(tweet)
            negative += 1
        elif pos > neg:
            positive_list.append(tweet)
            positive += 1

        elif pos == neg:
            neutral_list.append(tweet)
            neutral += 1

    positive = percentage(positive, noOfTweet)
    negative = percentage(negative, noOfTweet)
    neutral = percentage(neutral, noOfTweet)
    polarity = percentage(polarity, noOfTweet)
    positive = format(positive, '.1f')
    negative = format(negative, '.1f')
    neutral = format(neutral, '.1f')
    return positive


def two_countries_happiness(**context):
    tweets = context['task_instance'].xcom_pull(task_ids='CollectTweets')
    country_one = tweets[0]
    country_one_tweets = [x[2] for x in country_one]
    country_two = tweets[1]
    country_two_tweets = [x[2] for x in country_two]
    # clean first:

    # get score of each country tweets
    country_one_score = tweets_happiness_score(country_one_tweets)
    country_two_score = tweets_happiness_score(country_two_tweets)

    return country_one_score, country_two_score


def store_data(**context):
    datetime_obj = dt.datetime
    results = context['task_instance'].xcom_pull(
        task_ids='two_countries_happiness')
    df = pd.read_csv(
        '/c/Users/Islam/airflow_final/tweets/res_tweets.csv', index_col="Country")
    df[datetime_obj.today()] = [results[0], results[1]]
    df['Happiness_Average'] = df.mean(axis=1)
    df.to_csv('/c/Users/Islam/airflow_final/tweets/res_tweets.csv')


t1 = PythonOperator(
    task_id='CollectTweets',
    provide_context=True,
    python_callable=CollectTweets,
    dag=dag,
)

t2 = PythonOperator(
    task_id='two_countries_happiness',
    provide_context=True,
    python_callable=two_countries_happiness,
    dag=dag,
)
t3 = PythonOperator(
    task_id='store_data',
    provide_context=True,
    python_callable=store_data,
    dag=dag,
)

# step 5 - define dependencies
t1 >> t2
t2 >> t3
