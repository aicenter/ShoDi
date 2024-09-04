import geojson

def load_geojson(filepath: str) -> geojson.feature.FeatureCollection:
    print(f"[{datetime.now().strftime('%H:%M:%S')}]: Loading geojson file from: {os.path.realpath(filepath)}")
    input_stream = open(filepath, encoding='utf8')
    json_dict = geojson.load(input_stream)
    return json_dict
