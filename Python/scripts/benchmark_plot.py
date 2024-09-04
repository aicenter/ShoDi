import pandas
import plotly.express as px

df = pandas.read_csv('benchmark_data.csv')
print(df)

fig = px.scatter(df, x='memory', y='time', text='label', labels='', color='label', log_x=True, log_y=True, title="Memory and time comparison of various methods on the graph of Washington, D.C.")

fig.update_traces(marker=dict(size=10, line=dict(width=2, color='DarkSlateGrey')))
fig.update_xaxes(linecolor="black", mirror=True, title_text='Memory required by the structure in KB')
fig.update_yaxes(linecolor="black", mirror=True, title_text='Average time for one query in μs')
fig.update_traces(textposition='top center')

fig.update_layout(showlegend=False)
fig.update_layout(
    title={
        'x': 0.5,
        'xanchor': 'center',
        'y': 0.85,
        'yanchor': 'top'
    }
)

fig.write_image("plot.png")
