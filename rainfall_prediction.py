import pandas as pd
import numpy as np

def get_rainfall_prediction(state,month):
    
    data = pd.read_csv("rainfall_india.csv")
    
    mask = data['SUBDIVISION'] == state
    pos = np.flatnonzero(mask)
    op = data.iloc[pos][month].mean()
    
    return op