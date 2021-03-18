import numpy as np
from PIL import Image,ImageEnhance
from tensorflow.keras.models import load_model
from tensorflow.compat.v1 import ConfigProto, InteractiveSession

config = ConfigProto()
config.gpu_options.allow_growth = True
config.gpu_options.per_process_gpu_memory_fraction = 0.6
sess = InteractiveSession(config=config)

def decode(pred):
    ans = ''
    for i in range(4):
        ans += str(np.argmax(pred[i]))
    return ans
    
def captcha(img):
    cnn = load_model('crack.h5')
    img =img.convert('L')
    enhancer = ImageEnhance.Contrast(img)
    contrast = 10
    img = enhancer.enhance(contrast)
    img = np.asarray(img)
    img = np.array([img])
    pred = cnn.predict(img)
    return decode(pred)
