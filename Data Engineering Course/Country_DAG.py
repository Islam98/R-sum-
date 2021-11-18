# step 1 - import modules
from airflow import DAG
from datetime import datetime
from datetime import date

# Operators; we need this to operate!
from airflow.operators.python_operator import PythonOperator

import pandas as pd
import numpy as np
import os
from sklearn.linear_model import LinearRegression
from sklearn.model_selection import KFold
from scipy import stats
from sklearn.preprocessing import MinMaxScaler

# step 2 - define default args
default_args = {
    'owner': 'airflow',
    'depends_on_past': False,
    'start_date': datetime(2020, 12, 24)
}


# step 3 - instantiate DAG
dag = DAG(
    'ETL-Country-DAG',
    default_args=default_args,
    description='Country datasets ETL',
    schedule_interval='@once',
)

# step 4 Define tasks

# Helper Methods


def regression(x, y, test):
    # train a linear regression model to predicit the null valies
    model = LinearRegression()
    model.fit(x, y)
    coefficient = model.score(x, y)
    return model.predict(test)


def clean_life_expectancy(df_life_expectancy):
    # taka a copy
    df_life_expectancy_copy = df_life_expectancy.copy(deep=True)

    # Rename the features
    df_life_expectancy_copy.rename(columns={'Life expectancy ': 'Life expectancy', 'Adult Mortality': 'Adult mortality', 'infant deaths': 'Infant deaths',
                                            'percentage expenditure': 'Percentage expenditure', ' BMI ': 'BMI',
                                            'under-five deaths ': 'Under-five deaths', 'Diphtheria ': 'Diphtheria', ' HIV/AIDS': 'HIV/AIDS',
                                            'GDP': 'GDP', ' thinness  1-19 years': 'Thinness 10-19 years',
                                            ' thinness 5-9 years': 'Thinness 5-9 years'}, inplace=True)

    # use the median of countries to fill the null values of the features.
    df_life_expectancy_copy['Alcohol'] = df_life_expectancy_copy.groupby(
        'Country')['Alcohol'].apply(lambda x: x.fillna(x.median()))
    df_life_expectancy_copy['Hepatitis B'] = df_life_expectancy_copy.groupby(
        'Country')['Hepatitis B'].apply(lambda x: x.fillna(x.median()))
    df_life_expectancy_copy['Life expectancy'] = df_life_expectancy_copy.groupby(
        'Country')['Life expectancy'].apply(lambda x: x.fillna(x.median()))
    df_life_expectancy_copy['Adult mortality'] = df_life_expectancy_copy.groupby(
        'Country')['Adult mortality'].apply(lambda x: x.fillna(x.median()))
    df_life_expectancy_copy['BMI'] = df_life_expectancy_copy.groupby(
        'Country')['BMI'].apply(lambda x: x.fillna(x.median()))
    df_life_expectancy_copy['Polio'] = df_life_expectancy_copy.groupby(
        'Country')['Polio'].apply(lambda x: x.fillna(x.median()))
    df_life_expectancy_copy['Total expenditure'] = df_life_expectancy_copy.groupby(
        'Country')['Total expenditure'].apply(lambda x: x.fillna(x.median()))
    df_life_expectancy_copy['Diphtheria'] = df_life_expectancy_copy.groupby(
        'Country')['Diphtheria'].apply(lambda x: x.fillna(x.median()))
    df_life_expectancy_copy['GDP'] = df_life_expectancy_copy.groupby(
        'Country')['GDP'].apply(lambda x: x.fillna(x.median()))
    df_life_expectancy_copy['Thinness 10-19 years'] = df_life_expectancy_copy.groupby(
        'Country')['Thinness 10-19 years'].apply(lambda x: x.fillna(x.median()))
    df_life_expectancy_copy['Thinness 5-9 years'] = df_life_expectancy_copy.groupby(
        'Country')['Thinness 5-9 years'].apply(lambda x: x.fillna(x.median()))
    df_life_expectancy_copy['Income composition of resources'] = df_life_expectancy_copy.groupby(
        'Country')['Income composition of resources'].apply(lambda x: x.fillna(x.median()))
    df_life_expectancy_copy['Schooling'] = df_life_expectancy_copy.groupby(
        'Country')['Schooling'].apply(lambda x: x.fillna(x.median()))

    # drop the records which has relatively small number of nulls on the features
    life_ecpectancy_null_index = df_life_expectancy_copy[df_life_expectancy_copy['Life expectancy'].isnull(
    )].index
    df_life_expectancy_copy = df_life_expectancy_copy.drop(
        life_ecpectancy_null_index)
    adult_mortality_null_index = df_life_expectancy_copy[df_life_expectancy_copy['Adult mortality'].isnull(
    )].index
    df_life_expectancy_copy = df_life_expectancy_copy.drop(
        adult_mortality_null_index)
    alcohol_null_index = df_life_expectancy_copy[df_life_expectancy_copy['Alcohol'].isnull(
    )].index
    df_life_expectancy_copy = df_life_expectancy_copy.drop(alcohol_null_index)
    BMI_null_index = df_life_expectancy_copy[df_life_expectancy_copy['BMI'].isnull(
    )].index
    df_life_expectancy_copy = df_life_expectancy_copy.drop(BMI_null_index)
    total_expenditure_null_index = df_life_expectancy_copy[df_life_expectancy_copy['Total expenditure'].isnull(
    )].index
    df_life_expectancy_copy = df_life_expectancy_copy.drop(
        total_expenditure_null_index)
    thinness_10_19_years_null_index = df_life_expectancy_copy[
        df_life_expectancy_copy['Thinness 10-19 years'].isnull()].index
    df_life_expectancy_copy = df_life_expectancy_copy.drop(
        thinness_10_19_years_null_index)
    thinness_5_9_years_null_index = df_life_expectancy_copy[
        df_life_expectancy_copy['Thinness 5-9 years'].isnull()].index
    df_life_expectancy_copy = df_life_expectancy_copy.drop(
        thinness_5_9_years_null_index)

    # now we see the correlation between two columns.
    # for example, column X is highly correlated to column Y.
    # Now, if column X has null values and column Y is filled fully, then column X can be filled using column Y
    # We used linear regression

    # Hepatitis B <== Diphtheria
    x = df_life_expectancy_copy[df_life_expectancy_copy['Hepatitis B'].notnull(
    )]['Diphtheria'].values.reshape((-1, 1))
    y = df_life_expectancy_copy[df_life_expectancy_copy['Hepatitis B'].notnull(
    )]['Hepatitis B']
    test = df_life_expectancy_copy[df_life_expectancy_copy['Hepatitis B'].isnull(
    )]['Diphtheria'].values.reshape((-1, 1))
    y_pred = regression(x, y, test)
    df_life_expectancy_copy.loc[df_life_expectancy_copy['Hepatitis B'].isna(
    ), 'Hepatitis B'] = y_pred

    # GDP <== Percentage expenditure
    x = df_life_expectancy_copy[df_life_expectancy_copy['GDP'].notnull(
    )]['Percentage expenditure'].values.reshape((-1, 1))
    y = df_life_expectancy_copy[df_life_expectancy_copy['GDP'].notnull(
    )]['GDP']
    test = df_life_expectancy_copy[df_life_expectancy_copy['GDP'].isnull(
    )]['Percentage expenditure'].values.reshape((-1, 1))
    y_pred = regression(x, y, test)
    df_life_expectancy_copy.loc[df_life_expectancy_copy['GDP'].isna(
    ), 'GDP'] = y_pred

    # Population <== Infant deaths
    x = df_life_expectancy_copy[df_life_expectancy_copy['Population'].notnull(
    )]['Infant deaths'].values.reshape((-1, 1))
    y = df_life_expectancy_copy[df_life_expectancy_copy['Population'].notnull(
    )]['Population']
    test = df_life_expectancy_copy[df_life_expectancy_copy['Population'].isnull(
    )]['Infant deaths'].values.reshape((-1, 1))
    y_pred = regression(x, y, test)
    df_life_expectancy_copy.loc[df_life_expectancy_copy['Population'].isna(
    ), 'Population'] = y_pred

    # Income composition of resources <== Life expectancy
    x = df_life_expectancy_copy[df_life_expectancy_copy['Income composition of resources'].notnull(
    )]['Life expectancy'].values.reshape((-1, 1))
    y = df_life_expectancy_copy[df_life_expectancy_copy['Income composition of resources'].notnull(
    )]['Income composition of resources']
    test = df_life_expectancy_copy[df_life_expectancy_copy['Income composition of resources'].isnull(
    )]['Life expectancy'].values.reshape((-1, 1))
    y_pred = regression(x, y, test)
    df_life_expectancy_copy.loc[df_life_expectancy_copy['Income composition of resources'].isna(
    ), 'Income composition of resources'] = y_pred

    # Schooling <== Life expectancy
    x = df_life_expectancy_copy[df_life_expectancy_copy['Schooling'].notnull(
    )]['Life expectancy'].values.reshape((-1, 1))
    y = df_life_expectancy_copy[df_life_expectancy_copy['Schooling'].notnull(
    )]['Schooling']
    test = df_life_expectancy_copy[df_life_expectancy_copy['Schooling'].isnull(
    )]['Life expectancy'].values.reshape((-1, 1))
    y_pred = regression(x, y, test)
    df_life_expectancy_copy.loc[df_life_expectancy_copy['Schooling'].isna(
    ), 'Schooling'] = y_pred

    return df_life_expectancy_copy


def clean_country(df_country_data):
    df_country_data_copy = df_country_data.copy()

    # get the float number and ignore any strings in "Real Growth Rating(%)",     "Literacy Rate(%)", "Inflation(%)", and "Unemployement(%)" columns
    df_country_data_copy['Real Growth Rating(%)'] = df_country_data_copy['Real Growth Rating(%)'].str.extract(
        '((?:|[-+*/])-?\d+(?:\.\d+)?)', expand=False).astype(float)
    df_country_data_copy['Literacy Rate(%)'] = df_country_data_copy['Literacy Rate(%)'].str.extract(
        '((?:|[-+*/])-?\d+(?:\.\d+)?)', expand=False).astype(float)
    df_country_data_copy['Inflation(%)'] = df_country_data_copy['Inflation(%)'].str.extract(
        '((?:|[-+*/])-?\d+(?:\.\d+)?)', expand=False).astype(float)
    df_country_data_copy['Unemployement(%)'] = df_country_data_copy['Unemployement(%)'].str.extract(
        '((?:|[-+*/])-?\d+(?:\.\d+)?)', expand=False).astype(float)

    # get the null records of 'gini' column
    df_country_data_nulls_gini = df_country_data_copy[df_country_data_copy['gini'].isnull(
    )]
    # get the gini values of countries lie in the same subregion of each country has a null value in gini
    for index, null_row in df_country_data_nulls_gini.iterrows():
        df_country_data_close_subregion = df_country_data_copy[(
            (df_country_data_copy['subregion'] == null_row.subregion) & (df_country_data_copy['gini'].notnull()))]
        # check if there is countries resulted that has a gini value and in the same subregion for the current country iterated, if so then replace the null value with the average of the collected gini values
        if len(df_country_data_close_subregion['gini']) > 0:
            df_country_data_nulls_gini.loc[index,
                                           'gini'] = df_country_data_close_subregion['gini'].mean()
    # replace the null record with the update non-null
    df_country_data_copy.loc[df_country_data_nulls_gini.index,
                             'gini'] = df_country_data_nulls_gini[:]

    # replace the rest of null records data with zeros
    df_country_data_copy['gini'].fillna(0, inplace=True)
    # round the float value to 1
    df_country_data_copy.gini = df_country_data_copy.gini.round(1)

    # Do the same in the following columns that has null records. (Literacy Rate, Real Growth Rating)
    df_country_data_nulls_literacy = df_country_data_copy[df_country_data_copy['Literacy Rate(%)'].isnull(
    )]
    for index, null_row in df_country_data_nulls_literacy.iterrows():
        df_country_data_close_subregion = df_country_data_copy[(
            (df_country_data_copy['subregion'] == null_row.subregion) & (df_country_data_copy['Literacy Rate(%)'].notnull()))]
        if len(df_country_data_close_subregion['Literacy Rate(%)']) > 0:
            df_country_data_nulls_literacy.loc[index,
                                               'Literacy Rate(%)'] = df_country_data_close_subregion['Literacy Rate(%)'].mean()
    df_country_data_copy.loc[df_country_data_nulls_literacy.index,
                             'Literacy Rate(%)'] = df_country_data_nulls_literacy[:]
    df_country_data_copy['Literacy Rate(%)'] = df_country_data_copy['Literacy Rate(%)'].round(
        1)

    df_country_data_nulls_RGR = df_country_data_copy[df_country_data_copy['Real Growth Rating(%)'].isnull(
    )]
    for index, null_row in df_country_data_nulls_RGR.iterrows():
        df_country_data_close_subregion = df_country_data_copy[(
            (df_country_data_copy['subregion'] == null_row.subregion) & (df_country_data_copy['Real Growth Rating(%)'].notnull()))]
        if len(df_country_data_close_subregion['Real Growth Rating(%)']) > 0:
            df_country_data_nulls_RGR.loc[index,
                                          'Real Growth Rating(%)'] = df_country_data_close_subregion['Real Growth Rating(%)'].mean()
    df_country_data_copy.loc[df_country_data_nulls_RGR.index,
                             'Real Growth Rating(%)'] = df_country_data_nulls_RGR[:]
    df_country_data_copy['Real Growth Rating(%)'] = df_country_data_copy['Real Growth Rating(%)'].round(
        1)

    # Drop Inflation and Unmployment as it has 40% nulls
    df_country_data_copy.dropna(subset=["subregion"], inplace=True)
    df_country_data_copy.drop(
        ['Unemployement(%)', 'Inflation(%)'], inplace=True, axis=1)

    # predict the missing values of area given the populations
    x = df_country_data_copy[df_country_data_copy['area'].notnull(
    )]['population'].values.reshape((-1, 1))
    y = df_country_data_copy[df_country_data_copy['area'].notnull()]['area']
    model = LinearRegression()
    model.fit(x, y)
    r_sq = model.score(x, y)
    test = df_country_data_copy[df_country_data_copy['area'].isnull(
    )]['population'].values.reshape((-1, 1))
    y_pred = model.predict(test)

    # the y_pred value is too small for accuracy to replace the null value with these predicted values, by configuring the null values of are in the data we will drop the null values as the name of countries is not clear and will not add anything to exploration.
    df_country_data_copy.dropna(subset=["area"], inplace=True)

    # rename columns
    df_country_data_copy.rename(columns={'name': 'Name', 'region': 'Region', 'subregion': 'Subregion', 'population': 'Population', 'area': 'Area',
                                         'gini': 'Gini', 'Real Growth Rating(%) ': 'Real Growth Rating(%)', 'Literacy Rate(%)': 'Literacy Rate(%)',
                                         'Inflation(%)': 'Inflation(%)', 'Unemployement(%)': 'Unemployment(%)'}, inplace=True)

    return df_country_data_copy


def clean_happiness(df_Happiness_1, df_Happiness_2, df_Happiness_3, df_Happiness_4, df_Happiness_5):
    # Step 1: First need to add the year column to every dataframe of happiness dataset
    year = [2015]*158
    df_Happiness_1['Year'] = year
    year = [2016]*157
    df_Happiness_2['Year'] = year
    year = [2017]*155
    df_Happiness_3['Year'] = year
    year = [2018]*156
    df_Happiness_4['Year'] = year
    year = [2019]*156
    df_Happiness_5['Year'] = year
    # Step 2: Then we are renaming the coluns to unify the columns names for merging
    df_Happiness_3 = df_Happiness_3.rename(columns={'Happiness.Rank': 'Happiness Rank', 'Happiness.Score': 'Happiness Score',
                                                    'Economy..GDP.per.Capita.': 'Economy (GDP per Capita)', 'Health..Life.Expectancy.': 'Health (Life Expectancy)',
                                                    'Trust..Government.Corruption.': 'Trust (Government Corruption)',
                                                    'Dystopia.Residual': 'Dystopia Residual'})
    df_Happiness_4 = df_Happiness_4.rename(columns={'Overall rank': 'Happiness Rank', 'Score': 'Happiness Score',
                                                    'GDP per capita': 'Economy (GDP per Capita)',
                                                    'Healthy life expectancy': 'Health (Life Expectancy)',
                                                    'Freedom to make life choices': 'Freedom',
                                                    'Perceptions of corruption': 'Trust (Government Corruption)',
                                                    'Social support': 'Family', 'Country or region': 'Country'})
    df_Happiness_5 = df_Happiness_5.rename(columns={'Social support': 'Family', 'Overall rank': 'Happiness Rank', 'Score': 'Happiness Score', 'GDP per capita': 'Economy (GDP per Capita)',
                                                    'Healthy life expectancy': 'Health (Life Expectancy)', 'Country or region': 'Country', 'Freedom to make life choices': 'Freedom', 'Perceptions of corruption': 'Trust (Government Corruption)'})
    # Step 3:After unifying names we could merge the datasets by simply concatenating the frames
    frames = [df_Happiness_1, df_Happiness_2,
              df_Happiness_3, df_Happiness_4, df_Happiness_5]
    df_Happiness = pd.concat(frames, ignore_index=True)

    # Step 4: We removed the columns that contribute alot of nulls and isnot considered in analysis
    df_Happiness = df_Happiness.drop(columns=[
                                     'Lower Confidence Interval', 'Upper Confidence Interval', 'Whisker.high', 'Whisker.low', 'Standard Error'])
    # Step 5: As region is an important attribute in our analysis we chave created a dataframe containing unique countries to map them to the region
    # then used this dataframe to fill nulls in region
    country = df_Happiness[["Country", "Region"]]
    xn = country.drop_duplicates(subset=['Country'])
    df_Happiness = pd.merge(df_Happiness.drop(columns=['Region']), xn)
    # We have end up with 8 countries with no specified region so we dropped them as they are less than 2% of the whole dataset furthermore one
    # country entry ith null Trust was dropped
    df_Happiness = df_Happiness[df_Happiness['Region'].notna()]
    df_Happiness = df_Happiness[df_Happiness['Trust (Government Corruption)'].notna(
    )]
    # Step 6: Now we Dystopia Residual counted for alot of missing values so replacing it with median would totally skew the dataset so instead we used
    # regressionmodel to impute it using happiness score as it is the attribute with the highest correlation
    y_pred = []
    y_true = []
    df_filter = df_Happiness[df_Happiness['Dystopia Residual'] > 0].copy()
    kf = KFold(n_splits=10, shuffle=True)
    for train_index, test_index in kf.split(df_filter):
        df_test = df_filter.iloc[test_index]
        df_train = df_filter.iloc[train_index]
    for train_index, test_index in kf.split(df_filter):
        X_train = np.array(df_train['Happiness Score']).reshape(-1, 1)
        y_train = np.array(df_train['Dystopia Residual']).reshape(-1, 1)
        X_test = np.array(df_test['Happiness Score']).reshape(-1, 1)
        y_test = np.array(df_test['Dystopia Residual']).reshape(-1, 1)
    for train_index, test_index in kf.split(df_filter):
        model = LinearRegression()
        model.fit(X_train, y_train)
    for train_index, test_index in kf.split(df_filter):
        y_pred.append(model.predict(X_test)[0])
        y_true.append(y_test[0])
    # Step 7: After training the model we applied it on nulls to impute
    df_Happiness['Dystopia Residual'] = np.where(df_Happiness["Dystopia Residual"].isnull(), model.predict(
        np.array(df_Happiness["Happiness Score"]).reshape(-1, 1))[0][0], df_Happiness['Dystopia Residual'])

    # Step 8: Finally we have investigated each column for outliers and the columns that had outliers were imputed using median
    Q2 = df_Happiness['Family'].quantile(0.5)
    Q1 = df_Happiness['Family'].quantile(0.25)
    Q3 = df_Happiness['Family'].quantile(0.75)
    IQR = Q3 - Q1
    arr_1 = df_Happiness['Family'] < (Q1 - 1.5 * IQR)
    arr_2 = df_Happiness['Family'] > (Q3 + 1.5 * IQR)
    mask = (arr_1 | arr_2)
    df_Happiness['Family'] = np.where(mask, Q2, df_Happiness['Family'])

    Q2 = df_Happiness['Generosity'].quantile(0.5)
    Q1 = df_Happiness['Generosity'].quantile(0.25)
    Q3 = df_Happiness['Generosity'].quantile(0.75)
    IQR = Q3 - Q1
    arr_1 = df_Happiness['Generosity'] < (Q1 - 1.5 * IQR)
    arr_2 = df_Happiness['Generosity'] > (Q3 + 1.5 * IQR)
    mask = (arr_1 | arr_2)
    df_Happiness['Generosity'] = np.where(mask, Q2, df_Happiness['Generosity'])

    return df_Happiness


def integrate_datasets(df_clean_life_expectancy, df_clean_country, df_clean_happiness):
    # get the sub region from happiness dataset
    df_happiness_Country_SubRegion = df_clean_happiness[[
        'Country', 'Region']].rename(columns={"Region": "Sub Region"})
    df_happiness_Country_SubRegion = df_happiness_Country_SubRegion.drop_duplicates(
        subset='Country')

    # get the needed feature from each dataset to answer our reseatch question
    df_happiness_median = df_clean_happiness.groupby('Country').median().reset_index(
    )[["Country", "Happiness Score", "Trust (Government Corruption)", "Freedom"]]
    df_life_expectancy_median = df_clean_life_expectancy.groupby('Country').median(
    ).reset_index()[["Country", "Schooling", "Alcohol", "BMI", "Adult mortality"]]
    df_country_chosen_attribute = df_clean_country[[
        "Name", "Literacy Rate(%)", "Gini", "Region", "Real Growth Rating(%)"]].rename(columns={"Name": "Country"})

    # merge happiness and life expectancy datasets based on Country feature
    df_happiness_life_expectancey = pd.merge(
        df_life_expectancy_median, df_happiness_median, on="Country")
    df_happiness_life_expectancey = pd.merge(
        df_happiness_life_expectancey, df_happiness_Country_SubRegion, on="Country")

    # merge country dataset based on Country feature
    df_all_data = pd.merge(df_happiness_life_expectancey,
                           df_country_chosen_attribute, on="Country")

    return df_all_data


def feature_engineering(df_all_data):
    # BMI discretization
    df_all_data['eval'] = [np.nan]*136
    arr_1 = df_all_data['BMI'] > 18.5
    arr_2 = df_all_data['BMI'] < 24.9
    mask = (arr_1 & arr_2)
    df_all_data['eval'] = np.where(
        df_all_data['BMI'] <= 18.5, "underweight", df_all_data['eval'])
    df_all_data['eval'] = np.where(
        df_all_data['BMI'] >= 24.9, "overweight", df_all_data['eval'])
    df_all_data['eval'] = np.where(mask, "healthy", df_all_data['eval'])

    # Region One-hot Encoding
    df_all_data_encoded = pd.get_dummies(df_all_data['Region'])
    df_all_data = pd.concat([df_all_data, df_all_data_encoded], axis=1)

    # Normalize Literacy Rate
    df_country_normalize_literacy = df_all_data['Literacy Rate(%)']
    normalized_pledges = stats.boxcox(df_country_normalize_literacy)[0]
    df_all_data['Literacy Rate(%)'] = normalized_pledges

    # Adult mortality scaling
    adult_mortality_original = df_all_data[['Adult mortality']]
    adult_mortality_scaled = MinMaxScaler().fit_transform(adult_mortality_original)
    df_all_data['Adult mortality'] = adult_mortality_scaled

    return df_all_data

# Callable Methodds


def extract_data(**kwargs):
    # Read the datasets
    df_life_expectancy = pd.read_csv(
        '/c/Users/Islam/airflow_test/data/Life Expectancy Data.csv')
    df_country_data = pd.read_csv(
        '/c/Users/Islam/airflow_test/data/250 Country Data.csv', index_col=0)
    df_Happiness_1 = pd.read_csv(
        '/c/Users/Islam/airflow_test/data/Happiness_Dataset/2015.csv')
    df_Happiness_2 = pd.read_csv(
        '/c/Users/Islam/airflow_test/data/Happiness_Dataset/2016.csv')
    df_Happiness_3 = pd.read_csv(
        '/c/Users/Islam/airflow_test/data/Happiness_Dataset/2017.csv')
    df_Happiness_4 = pd.read_csv(
        '/c/Users/Islam/airflow_test/data/Happiness_Dataset/2018.csv')
    df_Happiness_5 = pd.read_csv(
        '/c/Users/Islam/airflow_test/data/Happiness_Dataset/2019.csv')
    return [df_life_expectancy, df_country_data, df_Happiness_1, df_Happiness_2, df_Happiness_3, df_Happiness_4, df_Happiness_5]


def transform_data(**context):
    print("balabeso")
    df = context['task_instance'].xcom_pull(task_ids='extract_data')
    print(df)
    df_life_expectancy = df[0]
    df_country_data = df[1]
    df_Happiness_1 = df[2]
    df_Happiness_2 = df[3]
    df_Happiness_3 = df[4]
    df_Happiness_4 = df[5]
    df_Happiness_5 = df[6]

    # clean the data
    df_clean_life_expectancy = clean_life_expectancy(df_life_expectancy)
    df_clean_country = clean_country(df_country_data)
    df_clean_happiness = clean_happiness(
        df_Happiness_1, df_Happiness_2, df_Happiness_3, df_Happiness_4, df_Happiness_5)

    # integrate the datasets
    df_all_data = integrate_datasets(
        df_clean_life_expectancy, df_clean_country, df_clean_happiness)

    # feature engineering
    df_all_data = feature_engineering(df_all_data)

    return df_all_data


def load_data(**context):
    df = context['task_instance'].xcom_pull(task_ids='transform_data')
    # save the data
    df.to_csv("/c/Users/Islam/airflow_test/data/all_country_data.csv")


# Tasks
t1 = PythonOperator(
    task_id='extract_data',
    provide_context=True,
    python_callable=extract_data,
    dag=dag,
)

t2 = PythonOperator(
    task_id='transform_data',
    provide_context=True,
    python_callable=transform_data,
    dag=dag,
)

t3 = PythonOperator(
    task_id='load_data',
    provide_context=True,
    python_callable=load_data,
    dag=dag,
)

# step 5 - define dependencies
t1 >> t2
t2 >> t3
