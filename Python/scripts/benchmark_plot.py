#
# MIT License
#
# Copyright (c) 2024 Czech Technical University in Prague
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.#
import pandas
import plotly.express as px

INSTANCE = "Chicago"
LONG_NAME = "Chicago (152,653 nodes)"

df = pandas.read_csv(f"benchmark_{INSTANCE}.csv")

fig = px.scatter(
    df, x="memory", y="time", text="label", labels="", log_x=True, log_y=True, template="simple_white",
    title=f"""Memory and time comparison of various methods<br>on the graph of {LONG_NAME}""")

fig.update_traces(marker=dict(size=10, color="powderblue", line=dict(width=2, color="DarkSlateGrey")))
fig.update_xaxes(linecolor="black", mirror=True, title_text="Memory allocated by the program in MiB", tickformat = ".1r")
fig.update_yaxes(linecolor="black", mirror=True, title_text="Average time for one query in μs")
fig.update_traces(textposition="bottom left")

fig.update_layout(showlegend=False)
fig.update_layout(
    title={
        "x": 0.5,
        "xanchor": "center",
        "y": 0.9,
        "yanchor": "top"
    }
)

fig.write_image(f"{INSTANCE}.png")
