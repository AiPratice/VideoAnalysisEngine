import yaml
import codecs

class Config(object):
    def __init__(self, path):
        self.path = path
        fd = codecs.open(path, 'r', 'utf-8')
        self.yaml_config = yaml.load(fd.read())
        fd.close()

    def get_yaml_config(self):
        return self.yaml_config
