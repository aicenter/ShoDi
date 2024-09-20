import pandas
import plotly.express as px

INSTANCE = "DC"
LONG_NAME = "Washington, D. C."

df = pandas.read_csv(f"benchmark_{INSTANCE}.csv")

fig = px.scatter(
    df, x="memory", y="time", text="label", labels="", log_x=True, log_y=True, template="simple_white",
    title=f"Memory and time comparison of various methods on the graph of {LONG_NAME}")

fig.update_traces(marker=dict(size=10, color="powderblue", line=dict(width=2, color="DarkSlateGrey")))
fig.update_xaxes(linecolor="black", mirror=True, title_text="Memory required by the structure in MiB", tickformat = ".1r")
fig.update_yaxes(linecolor="black", mirror=True, title_text="Average time for one query in μs")
fig.update_traces(textposition="bottom left")

fig.update_layout(showlegend=False)
fig.update_layout(
    title={
        "x": 0.5,
        "xanchor": "center",
        "y": 0.85,
        "yanchor": "top"
    }
)

fig.write_image(f"{INSTANCE}.png")
