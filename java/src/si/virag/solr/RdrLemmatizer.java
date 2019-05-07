/******************************************************************************
This file is part of the lemmagen library. It gives support for lemmatization.
Copyright (C) 2011 Jernej Virag <jernej@virag.si>

The lemmagen library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
******************************************************************************/

package si.virag.solr;

import java.io.IOException;
import java.nio.ByteBuffer;

import org.apache.lucene.analysis.TokenFilter;
import org.apache.lucene.analysis.TokenStream;
import org.apache.lucene.analysis.tokenattributes.CharTermAttribute;
import org.apache.lucene.analysis.tokenattributes.TypeAttribute;

import si.virag.lemmatizer.SlLemmatizer;

public class RdrLemmatizer extends TokenFilter 
{
	private static SlLemmatizer lemmatizer = null;
	
	private final TypeAttribute typeAtt = addAttribute(TypeAttribute.class);
	private final CharTermAttribute termAtt = addAttribute(CharTermAttribute.class);

	private ByteBuffer inputBuffer;
	private ByteBuffer outputBuffer;
	
	protected RdrLemmatizer(TokenStream input, String dictionaryPath) 
	{
		super(input);
		if (lemmatizer == null)
		{
			lemmatizer = new SlLemmatizer(dictionaryPath);
		}
		
		inputBuffer = ByteBuffer.allocateDirect(128);
		outputBuffer = ByteBuffer.allocateDirect(128);
	}
	
	@Override
	public boolean incrementToken() throws IOException 
	{
		if (this.input.incrementToken())
		{
			this.processBuffer(termAtt.buffer(), termAtt.length());
			return true;
		}
		
		return false;
	}
	
	private void ensureBufferSpace(int length)
	{
		// Make sure enough space is available in the buffer, consider worst-case scenario of 4-byte
		// characters.
		if (inputBuffer.capacity() < (4 * length) + 1)
		{
			inputBuffer = ByteBuffer.allocateDirect(4 * length + 1);
		}
		
		if (outputBuffer.capacity() < (4 * length) + 1)
		{
			outputBuffer = ByteBuffer.allocateDirect(4 * length + 1);
		}
	}
	
	protected void processBuffer(char[] buffer, int bufferLength)
	{
		if (typeAtt.equals("<HOST>") || 
			typeAtt.equals("<EMAIL>") ||
			bufferLength < 4) 
		{
			return;
		}
		
		ensureBufferSpace(bufferLength);
		inputBuffer.rewind();
		UnicodeUtil.UTF16toUTF8(buffer, bufferLength, inputBuffer);
		// NUL terminate the string
		inputBuffer.put((byte)0x0);
		int byteLength = lemmatizer.lemmatize(inputBuffer, outputBuffer);
		outputBuffer.rewind();
		UnicodeUtil.UTF8toUTF16(outputBuffer, byteLength, termAtt);
	}
}

