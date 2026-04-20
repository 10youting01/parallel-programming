import numpy as np
import pandas as pd

originals = np.arange(0, 256)
rng = np.random.default_rng()
substitutes = rng.permutation(256)

pairs = pd.DataFrame(columns=['Originals', 'Substitutes'])
pairs['Originals'] = originals
pairs['Substitutes'] = substitutes
print(pairs)

pairs = pairs.reindex(np.random.permutation(pairs.index))
print(pairs)

print('substitutes = {', ', '.join(pairs['Substitutes'].map(lambda x: str(x))), '};')
print('originals = {', ', '.join(pairs['Originals'].map(lambda x: str(x))), '};')

print('direct_substitute = {', ', '.join(map(lambda x: str(x), substitutes)), '};')
